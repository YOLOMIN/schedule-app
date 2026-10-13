#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QListWidgetItem>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDateTime>
#include <QTimer>
#include <QStyle>
#include <QAction>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 打开数据库文件（放在程序目录下，SQLite 数据库就是一个文件）
    if (!m_db.open("schedule.db")) {
        QMessageBox::critical(this, "错误", "无法打开数据库文件 schedule.db");
    }

    // 时间选择器默认设为「1 小时后」，显示格式友好
    ui->dueTimeEdit->setDateTime(QDateTime::currentDateTime().addSecs(3600));
    ui->dueTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm");

    // 信号与槽：点击按钮 → onAddClicked；输入框回车 → 也触发添加
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(ui->titleEdit, &QLineEdit::returnPressed, this, &MainWindow::onAddClicked);

    // 启动时加载已有日程
    refreshList();

    QTimer *timer =new QTimer(this);
    timer->start(60000);
    connect(timer,&QTimer::timeout,this,&MainWindow::checkDueTasks);

    QIcon tempIcon = QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation);

    m_tray = new QSystemTrayIcon(tempIcon);
    m_trayMenu = new QMenu(this);

    QAction *showAct = m_trayMenu->addAction("显示主界面");
    QAction *quitAct = m_trayMenu->addAction("退出");

    connect(showAct,&QAction::triggered,this,[this]{show();});
    connect(quitAct,&QAction::triggered,qApp,&QApplication::quit);

    m_tray->setContextMenu(m_trayMenu);
    m_tray->show();

    m_client = new OpenClawClient(this);

    connect(ui->sendButton,&QPushButton::clicked,this,&MainWindow::onSendClicked);

    connect(m_client,&OpenClawClient::replyReceived,this,&MainWindow::onReplyReceived);

    loadTodayPlan();
    loadGoals();

    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();

}

void MainWindow::loadTodayPlan()
{
    QString path = "C:/Users/Administrator/.openclaw/workspace/memory/reminder_today.txt";

    QFile file(path);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream in(&file);
        ui->planDisplay->setPlainText(in.readAll());
        file.close();

    }else{
        ui->planDisplay->setPlainText("暂无今日计划（找不到文件）");
    }
}

void MainWindow::loadGoals()
{
    QString path = "C:/Users/Administrator/.openclaw/workspace/memory/长期任务.md";

    QFile file(path);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream in(&file);
        ui->goalDisplay->setPlainText(in.readAll());
        file.close();

    }else{
       ui->goalDisplay->setPlainText("暂无长期任务（找不到文件）");
    }
}

void MainWindow::onAddClicked()
{
    QString title = ui->titleEdit->text().trimmed();
    if (title.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入日程标题");
        return;
    }

    QString due = ui->dueTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss");

    if (m_db.addTask(title, due)) {
        ui->titleEdit->clear();   // 添加成功后清空输入框
        refreshList();
    } else {
        QMessageBox::warning(this, "提示", "添加失败，请检查数据库");
    }
}

void MainWindow::refreshList()
{
    ui->taskList->clear();

    QVector<Task> tasks = m_db.allTasks();
    for (const Task &t : tasks) {
        // 每行一个 QListWidgetItem，用 UserRole 存数据库 id
        QListWidgetItem *item = new QListWidgetItem(ui->taskList);
        item->setData(Qt::UserRole, t.id);

        // 每行放一个自定义 widget：勾选框 + 标题时间 + 删除按钮
        QWidget *row = new QWidget;
        QHBoxLayout *lay = new QHBoxLayout(row);
        lay->setContentsMargins(6, 2, 6, 2);

        QCheckBox *check = new QCheckBox;
        check->setText(t.title + "　(" + t.dueTime.left(16) + ")");
        check->setChecked(t.done);
        lay->addWidget(check);

        lay->addStretch();

        QPushButton *del = new QPushButton("删除");
        lay->addWidget(del);

        row->setLayout(lay);
        item->setSizeHint(row->sizeHint());
        ui->taskList->setItemWidget(item, row);

        // 勾选变化 → 更新数据库 done 字段
        connect(check, &QCheckBox::toggled, this, [this, id = t.id](bool checked) {
            m_db.setTaskDone(id, checked);
        });

        // 删除按钮 → 删库 + 刷新列表
        connect(del, &QPushButton::clicked, this, [this, id = t.id]() {
            m_db.removeTask(id);
            refreshList();
        });
    }
}

void MainWindow::checkDueTasks()
{
    QDateTime now = QDateTime::currentDateTime();
    QVector<Task> tasks = m_db.allTasks();

    for(const Task &t:tasks){

        if(t.done) continue;

        QDateTime due = QDateTime::fromString(t.dueTime,"yyyy-MM-dd HH:mm:ss");

        if(due > now) continue;

        if(m_notifiedIds.contains(t.id))
            continue;


        QString msg = QString("任务%1，的截至时间为%2，当前时间为%3").arg(t.title).arg(t.dueTime).arg(now.toString());
        QMessageBox::warning(this, "提示", msg);

        m_notifiedIds.insert(t.id);

    }
}

void MainWindow::onSendClicked()
{
    QString text = ui->chatInput->text().trimmed();

    if(text.isEmpty())
        return ;
    ui->chatDisplay->append("我："+text);

    ui->chatInput->clear();

    m_client->sendMessage(text);


}

void MainWindow::onReplyReceived(const QString &reply)
{
    ui->chatDisplay->append("OpenClaw:"+reply);
}
