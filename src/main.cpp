#include <QApplication>
#include <QSystemTrayIcon>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QCommandLineParser>
#include <QDebug>
#include <QTimer>
#include <QElapsedTimer>
#include <QEventLoop>
#include <QThread>
#include <QProcess>

bool wait4SystemTray(int timeout = 5000, int interval = 100) {
  QElapsedTimer timer;
  timer.start();
  QEventLoop loop;

  while (timer.elapsed() < timeout) {
    if (QSystemTrayIcon::isSystemTrayAvailable())
      return true;

    QTimer::singleShot(interval, &loop, &QEventLoop::quit);
    loop.exec();
  }

  return false;
}

int main(int argc, char *argv[]) {

  QString gioPath = QStandardPaths::findExecutable("gio");

  if (gioPath.isEmpty()) {
    qWarning() <<  "gio command not found ...";
    return 0;
  }

  QString configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/ordered-autostart";
  QFileInfo fi(configPath);
  if (!fi.exists()) {
    return 0;
  }

  QApplication app(argc, argv);

  if (!wait4SystemTray()) {
    qWarning() <<  "Please make sure your desktop environment supports tray icons ...";
  }

  QSettings settings(configPath, QSettings::IniFormat);
  settings.setIniCodec("UTF-8");

  settings.beginGroup("config");
  int startupDelay = settings.value("delay", 0).toInt();
  settings.endGroup();

  if(startupDelay) {
    QThread::msleep(startupDelay);
  }

  settings.beginGroup("autostart");
  QStringList keys = settings.childKeys();
  for (const QString &key : keys) {
    int enabled = settings.value(key,0).toInt();
    if (enabled) {
      QString cmd = "gio launch '/usr/share/applications/" + key + ".desktop' >/dev/null 2>/dev/null";
      QProcess::startDetached("sh", QStringList() << "-c" << cmd);
    }
  }
  settings.endGroup();

  return 0;
}
