#ifndef UPDATENOTIFIER_HPP
#define UPDATENOTIFIER_HPP
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <generated/version.hpp>

namespace Telemetry {
class UpdateNotifier : public QObject {
  Q_OBJECT
 public:
  UpdateNotifier(bool useBeta);
  void checkUpdate();
  ~UpdateNotifier();
  void setBeta(bool value);

 private slots:

  void managerFinished(QNetworkReply* reply);

 private:
  QNetworkAccessManager* manager;
  QNetworkRequest request;
  QString currentVersionStr();
  bool beta;
};
}  // namespace Telemetry

#endif  // UPDATENOTIFIER_HPP
