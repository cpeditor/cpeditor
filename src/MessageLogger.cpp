#include <MessageLogger.hpp>
#include <QDateTime>

namespace Log {

QPlainTextEdit* MessageLogger::box = nullptr;

void MessageLogger::setContainer(QPlainTextEdit* value) {
  MessageLogger::box = value;
}

void MessageLogger::info(std::string head, std::string body) {
  std::string ans = "<b>[";
  int long long timestamp = QDateTime::currentSecsSinceEpoch();
  auto val = QDateTime::fromSecsSinceEpoch(timestamp).time();
  ans += val.toString().toStdString();
  ans += "] [";
  ans += head;
  ans += "] </b>";
  ans += "<font color=blue>[";
  ans += body;
  ans += "]</font>";

  box->appendHtml(QString::fromStdString(ans));
}
void MessageLogger::warn(std::string head, std::string body) {
  std::string ans = "<b>[";
  int long long timestamp = QDateTime::currentSecsSinceEpoch();
  auto val = QDateTime::fromSecsSinceEpoch(timestamp).time();
  ans += val.toString().toStdString();
  ans += "] [";
  ans += head;
  ans += "] </b>";
  ans += "<font color=green>[";
  ans += body;
  ans += "]</font>";

  box->appendHtml(QString::fromStdString(ans));
}
void MessageLogger::error(std::string head, std::string body) {
  std::string ans = "<b>[";
  auto timestamp = QDateTime::currentSecsSinceEpoch();
  auto val = QDateTime::fromSecsSinceEpoch(timestamp).time();
  ans += val.toString().toStdString();
  ans += "] [";
  ans += head;
  ans += "] </b>";
  ans += "<font color=red>[";
  ans += body;
  ans += "]</font>";

  box->appendHtml(QString::fromStdString(ans));
}
void MessageLogger::clear() {
  box->clear();
}

}  // namespace Log
