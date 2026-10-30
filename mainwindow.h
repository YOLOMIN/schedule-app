#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database.h"
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QSet>
#include "openclawclient.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
protected:
    void closeEvent(QCloseEvent *event) override;
    void loadTodayPlan();
    void loadGoals();

private slots:
    void onAddClicked();   // 点击「添加」按钮
    void refreshList();    // 从数据库重新加载列表到界面
    void checkDueTasks();
    void onSendClicked();
    void onReplyReceived(const QString &reply);
    void onTrayMessageClicked();
    void onErrorOccurred(const QString &err);
    void onNavChanged(int row);

private:
    Ui::MainWindow *ui;
    Database m_db;         // 数据库对象（成员，生命周期跟随窗口）
    QSystemTrayIcon *m_tray;
    QMenu *m_trayMenu;
    QSet<int> m_notifiedIds;    //已提醒过的任务id
    OpenClawClient *m_client;

};
#endif // MAINWINDOW_H
