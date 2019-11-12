#ifndef MESSAGELOGGER_HPP
#define MESSAGELOGGER_HPP
#include <QPlainTextEdit>
#include <string>

namespace Log {
class MessageLogger {
 public:
  MessageLogger() = delete;

  static void warn(std::string head, std::string body);
  static void info(std::string head, std::string body);
  static void error(std::string head, std::string body);
  static void clear();
  static void setContainer(QPlainTextEdit* value);

 private:
  static QPlainTextEdit* box;
};
}  // namespace Log

#endif  // MESSAGELOGGER_HPP
