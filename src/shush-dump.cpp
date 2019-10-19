#include "shush-dump.hpp"

shush::dump::DumpElement::
DumpElement(const char* const expression, const char* const file_name, 
            uint32_t line_number, const char* const msg) {
  LoadVars(expression, file_name, line_number, msg);
}

shush::dump::DumpElement::DumpElement(DumpElement& exc) {
  LoadVars(exc.expression, exc.file_name, exc.line_number, exc.msg);
}

shush::dump::DumpElement& shush::dump::DumpElement::operator=(
  const DumpElement& exc) {
  LoadVars(exc.expression, exc.file_name, exc.line_number, exc.msg);
  return *this;
}

shush::dump::DumpElement::DumpElement(DumpElement&& exc) noexcept {
  LoadVars(exc.expression, exc.file_name, exc.line_number, exc.msg);
}

shush::dump::DumpElement& shush::dump::DumpElement::operator=(
  DumpElement&& exc) noexcept {
  LoadVars(exc.expression, exc.file_name, exc.line_number, exc.msg);
  return *this;
}

void shush::dump::DumpElement::LoadVars(const char* const expression,
  const char* const file_name, const uint32_t line_number,
  const std::string& msg) {
  strcpy(this->expression, expression);
  strcpy(this->file_name, file_name);
  this->msg = msg;
  this->line_number = line_number;
}

shush::dump::Dump::Dump(const char* const expression,
  const char* const file_name, uint32_t line_number, 
  const char* const msg)
    : cur_size_(1) {
  dump_stack[0].LoadVars(expression, file_name, line_number, msg);
}

shush::dump::Dump::Dump(const char* const expression,
  const char* const file_name, uint32_t line_number, const std::string& msg)
    : cur_size_(1) {
  dump_stack[0].LoadVars(expression, file_name, line_number, msg);
}

shush::dump::Dump::Dump()
    : cur_size_(0) {}

void shush::dump::Dump::PushToStack(DumpElement&& exc) {
  dump_stack[cur_size_++] = std::move(exc);
}

void shush::dump::Dump::EmplaceToStack(const char* const expression,
  const char* const file_name, uint32_t line_number, const char* const msg) {
  dump_stack[cur_size_++].LoadVars(expression, file_name, line_number, msg);
}

shush::dump::DumpElement shush::dump::Dump::PopFromStack() {
  return dump_stack[--cur_size_];
}

bool shush::dump::Dump::Empty() {
  return cur_size_ == 0;
}

size_t shush::dump::Dump::Size() const {
  return cur_size_;
}

void shush::dump::HandleFinalDump(Dump& dump) {
  std::ofstream file;
  char file_name[FILE_NAME_SIZE];
  format::FormatString(DEFAULT_DUMP_NAME, file_name);
  file.open(file_name);

  std::cerr << "ERROR OCCURED. ABORTING THE PROGRAM\n";

  while (!dump.Empty()) {
    DumpElement elem = dump.PopFromStack();
    file << "- - - - - - - - - DUMP CAUGHT #" << dump.Size() << "- - - - - - - - - \n";
    if (elem.expression[0] != '\0') {
      file << "ASSERTION FAILED: " << elem.expression << "\n";
    }
    file << "Dump message: " << elem.msg << "\n";
    file << "Source: " << elem.file_name << "\n";
    file << "Line number: " << elem.line_number << "\n";
  }

  if (!file.bad()) {
    std::cerr << "DUMP FILE FORMED: " << file_name << std::endl;
  }

  file.close();
}

std::string shush::dump::GetBadGoodStr(bool exr) {
  return exr ? "(GOOD) " : "(BAD)  ";
}
