#include <MessageLogger.hpp>
#include <QJsonDocument>
#include <UpdateNotifier.hpp>

namespace Telemetry {
UpdateNotifier::UpdateNotifier(bool useBeta) {
  manager = new QNetworkAccessManager();
  QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this,
                   SLOT(managerFinished(QNetworkReply*)));
  beta = useBeta;
}
UpdateNotifier::~UpdateNotifier() {
  delete manager;
}
void UpdateNotifier::setBeta(bool value) {
  beta = value;
}

void UpdateNotifier::checkUpdate() {
  request.setUrl(QUrl(
      "https://api.github.com/repos/coder3101/cp-editor2/releases/latest"));
  manager->get(request);
}

QString UpdateNotifier::currentVersionStr() {
  std::string version = std::to_string(APP_VERSION_MAJOR) + "." +
                        std::to_string(APP_VERSION_MINOR) + "." +
                        std::to_string(APP_VERSION_PATCH);
  return QString::fromStdString(version);
}
void UpdateNotifier::managerFinished(QNetworkReply* reply) {
  if (reply->error()) {
    qDebug() << reply->errorString();
    return;
  }
  QString jsonReply = reply->readAll();
  QTextStream out(stdout);

  QJsonDocument doc = QJsonDocument::fromJson(jsonReply.toLocal8Bit());

  QString latestRelease = doc["tag_name"].toString();
  bool isBeta = doc["prerelease"].toBool();
  QString downloadUrl = doc["html_url"].toString();

  QList<QString> latestSplit = latestRelease.split(".");
  QList<QString> currenSplit = currentVersionStr().split(".");

  bool isUpdateAvailable =
      (currenSplit[0].toInt() < latestSplit[0].toInt()) ||  // Major update
      (currenSplit[1].toInt() < latestSplit[1].toInt()) ||  // Minor Update
      (currenSplit[2].toInt() < latestSplit[2].toInt());    // Patch Update

  qDebug() << isUpdateAvailable;

  bool showMessage = (isUpdateAvailable && (beta || !isBeta));
  if (showMessage) {
    Log::MessageLogger::info(
        "Updater", "A new update " + latestRelease.toStdString() +
                       " is available. <a href = " + downloadUrl.toStdString() +
                       ">Please Download" + "</a>");
  }
}
}  // namespace Telemetry
