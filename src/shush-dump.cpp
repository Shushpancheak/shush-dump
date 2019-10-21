#include "shush-dump.hpp"


shush::dump::DumpElement::
DumpElement(const char* const expression,
            const char* const file_name,
            const uint32_t    line_number,
            const char* const msg,
            const int         error_code,
            const char*       error_name) {
  LoadVars(expression, file_name, line_number, msg, error_code, error_name);
}


shush::dump::DumpElement::DumpElement(DumpElement& exc) {
  LoadVars(
      exc.expression, exc.file_name, exc.line_number, exc.msg, exc.error_code,
      exc.error_name);
}


shush::dump::DumpElement& shush::dump::DumpElement::operator=(
    const DumpElement& exc) {
  LoadVars(
      exc.expression, exc.file_name, exc.line_number, exc.msg, exc.error_code,
      exc.error_name);
  return *this;
}


shush::dump::DumpElement::DumpElement(DumpElement&& exc) noexcept {
  LoadVars(
      exc.expression, exc.file_name, exc.line_number, exc.msg, exc.error_code,
      exc.error_name);
}


shush::dump::DumpElement& shush::dump::DumpElement::operator=(
    DumpElement&& exc) noexcept {
  LoadVars(
      exc.expression, exc.file_name, exc.line_number, exc.msg, exc.error_code,
      exc.error_name);
  return *this;
}


void shush::dump::DumpElement::LoadVars(const char* const expression,
                                        const char* const file_name,
                                        const uint32_t    line_number,
                                        const char*       msg,
                                        int               error_code,
                                        const char*       error_name) {
  strcpy(this->expression, expression);
  strcpy(this->file_name, file_name);
  this->msg         = msg;
  this->line_number = line_number;
  this->error_code  = error_code;
  this->error_name  = error_name;
}


shush::dump::Dump::Dump(const char* const expression,
                        const char* const file_name,
                        const uint32_t    line_number,
                        const char* const msg,
                        const int         error_code,
                        const char*       error_name)
  : cur_size_(1) {
  dump_stack[0].LoadVars(
      expression, file_name, line_number, msg, error_code, error_name);
}


shush::dump::Dump::Dump()
  : cur_size_(0) {}


void shush::dump::Dump::PushToStack(DumpElement&& exc) {
  dump_stack[cur_size_++] = std::move(exc);
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
  char          file_name[FILE_NAME_SIZE];
  format::FormatString(DEFAULT_DUMP_NAME, file_name);
  FILE* file = fopen(file_name, "w");

  if (!file) {
    fprintf(stderr, "COULD NOT OPEN FILE \"%s\" TO CREATE DUMP.\n", file_name);
    return;
  }

  while (!dump.Empty()) {
    DumpElement elem = dump.PopFromStack();

    fprintf(file, "- - - - - - - - - DUMP CAUGHT #%zu - - - - - - - - - \n", dump.Size());

    fprintf(stderr, "Error: %d (%s).\n", elem.error_code, elem.error_name);

    fprintf(file, "Expression that caused error: %s\n", elem.expression);
    fprintf(file, "Source: %s\n",          elem.file_name);
    fprintf(file, "Line number: %u\n",     elem.line_number);
    fprintf(file, "Error code: %d (%s)\n", elem.error_code, elem.error_code == -2 ? STR_NO_ERROR :
     elem.error_code == -1 ? STR_ASSERTION_FAILED : elem.error_name);
    fprintf(file, "Dump message: %s\n",    elem.msg);
  }

  fprintf(stderr, "Dump file \"%s\" was formed .\n", file_name);

  fclose(file);
}


std::string shush::dump::GetBadGoodStr(bool expr) {
  return expr ? "(GOOD) " : "(BAD)  ";
}
