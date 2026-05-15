#pragma once

#include <QJsonObject>
#include <QWidget>

class QLabel;
class QPushButton;
class QComboBox;
class QPropertyAnimation;

class PackageDetailPane : public QWidget {
    Q_OBJECT
public:
    explicit PackageDetailPane(QWidget *parent = nullptr);

    void setPackage(const QJsonObject &pkg);

signals:
    void installRequested(const QString &package, const QString &source);

private:
    void buildUi();

    QJsonObject m_current;
    QLabel      *m_name        = nullptr;
    QLabel      *m_version     = nullptr;
    QLabel      *m_description = nullptr;
    QLabel      *m_meta        = nullptr;
    QComboBox   *m_sourcePicker = nullptr;
    QPushButton *m_installBtn  = nullptr;
};
