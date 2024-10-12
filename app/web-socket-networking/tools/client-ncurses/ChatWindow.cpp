/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include "ChatWindow.h"

#include <cassert>

#include <form.h>
#include <ncurses.h>

////////////////////////////////////////////////////////////////////////////////
// Hidden ChatWindow implementation
////////////////////////////////////////////////////////////////////////////////

class ChatWindowImpl {
public:
  ChatWindowImpl(std::function<void(std::string)> onTextEntry, int updateDelay);
  ~ChatWindowImpl();
  ChatWindowImpl(ChatWindowImpl&) = delete;
  ChatWindowImpl(ChatWindowImpl&&) = delete;
  ChatWindowImpl& operator=(ChatWindowImpl&) = delete;
  ChatWindowImpl& operator=(ChatWindowImpl&&) = delete;

  void resizeOnShapeChange();

  void processInput(int key);

  [[nodiscard]] size_t getFieldSize() const;

  [[nodiscard]] std::string getFieldString() const;

  void refreshWindow();

  void displayText(const std::string& text, MessageType type);

private:
  std::function<void(std::string)> onTextEntry;

  int parentX   = 0;
  int parentY   = 0;
  int entrySize = 3;

  WINDOW *view     = nullptr;
  WINDOW *entry    = nullptr;
  WINDOW *entrySub = nullptr;

  FIELD *fields[2]  = { nullptr, nullptr };
  FIELD *entryField = nullptr;

  FORM *entryForm = nullptr;
};


ChatWindowImpl::ChatWindowImpl(std::function<void(std::string)> onTextEntry,
                               int updateDelay)
  : onTextEntry{std::move(onTextEntry)} {
  initscr();
  start_color();
  noecho();
  halfdelay(updateDelay);

  // Initialize color pairs
  init_pair(static_cast<int>(MessageType::Other), COLOR_BLUE, COLOR_BLACK);    // Other people
  init_pair(static_cast<int>(MessageType::Self), COLOR_GREEN, COLOR_BLACK);   // Yourself
  init_pair(static_cast<int>(MessageType::Server), COLOR_YELLOW, COLOR_BLACK);  // Game server announcer
  init_pair(static_cast<int>(MessageType::GameState), COLOR_WHITE, COLOR_BLACK);   // Game state

  getmaxyx(stdscr, parentY, parentX);

  view = newwin(parentY - entrySize, parentX, 0, 0);
  scrollok(view, TRUE);

  entry = newwin(entrySize, parentX, parentY - entrySize, 0);
  wborder(entry, ' ', ' ', '-', ' ', '+', '+', ' ', ' ');
  entrySub = derwin(entry, entrySize - 1, parentX, 1, 0);
  
  entryField = new_field(entrySize - 1, parentX, 0, 0, 0, 0);
  assert(entryField && "Error creating entry field.");
  set_field_buffer(entryField, 0, "");
  set_field_opts(entryField, O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);

  fields[0] = entryField;
  entryForm = new_form(fields);
  assert(entryForm && "Error creating entry form.");
  set_form_win(entryForm, entry);
  set_form_sub(entryForm, entrySub);
  post_form(entryForm);

  refresh();
  wrefresh(entry);
}


ChatWindowImpl::~ChatWindowImpl() {
  unpost_form(entryForm);
  free_form(entryForm);
  free_field(entryField);
  delwin(entry);
  delwin(view);
  endwin();
}


void
ChatWindowImpl::resizeOnShapeChange() {
  int newX, newY;
  getmaxyx(stdscr, newY, newX);

  if (newY != parentY || newX != parentX) {
    parentX = newX;
    parentY = newY;

    wresize(view, parentY - entrySize, parentX);
    wresize(entry, entrySize, parentX);
    mvwin(entry, parentY - entrySize, 0);

    wclear(stdscr);
    wborder(entry, ' ', ' ', '-', ' ', '+', '+', ' ', ' ');
  }
}


void
ChatWindowImpl::processInput(int key) {
  switch(key) {
    case KEY_ENTER:
    case '\n':
      // Requesting validation synchs the seen field & the buffer.
      form_driver(entryForm, REQ_VALIDATION);
      onTextEntry(getFieldString());
      move(1, 1);
      set_field_buffer(entryField, 0, "");
      refresh();
      pos_form_cursor(entryForm);
      break;
    case KEY_BACKSPACE:
    case 127: //ASCII delete
      form_driver(entryForm, REQ_DEL_PREV);
      break;
    case KEY_DC:
      form_driver(entryForm, REQ_DEL_CHAR);
      break;
    case ERR:
      // swallow
      break;
    default:
      form_driver(entryForm, key);
      break;
  }
}


void
ChatWindowImpl::refreshWindow() {
  wrefresh(view);
  wrefresh(entry);
}


void
ChatWindowImpl::displayText(const std::string& text, MessageType type) {
  wattron(view, COLOR_PAIR(static_cast<int>(type)));
  wprintw(view, "%s", text.c_str());
  wattroff(view, COLOR_PAIR(static_cast<int>(type)));
  wrefresh(view);
}


size_t
ChatWindowImpl::getFieldSize() const {
  size_t x, y;
  getyx(entrySub, y, x);
  return y * parentX + x;
}


std::string
ChatWindowImpl::getFieldString() const {
  return std::string{field_buffer(entryField, 0), getFieldSize()};
}


////////////////////////////////////////////////////////////////////////////////
// ChatWindow API
////////////////////////////////////////////////////////////////////////////////


ChatWindow::ChatWindow(std::function<void(std::string)> onTextEntry,
                       int updateDelay)
  : impl{std::make_unique<ChatWindowImpl>(std::move(onTextEntry), updateDelay)}
    { }


ChatWindow::~ChatWindow() = default;


void
ChatWindow::update() {
  impl->resizeOnShapeChange();
  impl->processInput(getch());
  impl->refreshWindow();
}


void
ChatWindow::displayText(const std::string& text, MessageType type) {
  impl->displayText(text, type);
}


