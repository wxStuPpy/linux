```cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColorDialog>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QWizard>
#include <QVBoxLayout>
#include <QLabel>
#include <QButtonGroup>
#include <QRadioButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), _counter(0)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 颜色选择对话框
void MainWindow::on_colorBtn_clicked()
{
    // 创建颜色对话框，初始颜色为蓝色，允许透明度选择
    QColorDialog colorDlg(Qt::blue, this);
    colorDlg.setOption(QColorDialog::ShowAlphaChannel); // 启用Alpha通道（透明度）

    // 模态方式显示对话框（阻塞其他操作直到关闭）
    if (colorDlg.exec() == QDialog::Accepted) {
        QColor color = colorDlg.selectedColor(); // 获取用户选择的颜色
        qDebug() << "color is" << color;
    }
}

// 文件选择对话框
void MainWindow::on_txtBtn_clicked()
{
    QString path = QDir::currentPath(); // 获取当前工作目录
    QString title = tr("文件对话框");
    QString filter = tr("文本文件(*.txt);;图片文件(*.jpg *.gif *.png);;所有文件(*.*)");

    // 打开文件选择对话框，用户确认后返回所选文件路径
    QString fileName = QFileDialog::getOpenFileName(this, title, path, filter);
    if (!fileName.isEmpty()) { // 检查用户是否选择了文件
        qDebug() << "fileName:" << fileName;
    }
}

// 整数输入对话框
void MainWindow::on_pushButton_clicked()
{
    bool ok = false;
    // 获取整数输入（默认值200，范围-200~400，步长10）
    int intData = QInputDialog::getInt(
        this, tr("整数输入对话框"), tr("请输入数字"),
        200, -200, 400, 10, &ok
    );
    if (ok) { // 用户点击了"确定"按钮
        qDebug() << intData;
    }
}

// 浮点数输入对话框
void MainWindow::on_floatBtn_clicked()
{
    bool ok = false;
    // 获取浮点数输入（默认值1.671，范围-2.33~3.3，3位小数）
    double doubleData = QInputDialog::getDouble(
        this, tr("浮点数输入对话框"), tr("请输入浮点数"),
        1.671, -2.33, 3.3, 3, &ok
    );
    if (ok) { // 用户点击了"确定"按钮
        qDebug() << doubleData;
    }
}

// 条目选择对话框
void MainWindow::on_pushButton_2_clicked()
{
    QStringList items;
    items << tr("条目1") << tr("条目2");
    bool ok = false;

    // 获取下拉列表选择（可编辑，默认选中第0项）
    QString itemData = QInputDialog::getItem(
        this, tr("条目对话框"), tr("输入或选择条目"),
        items, 0, true, &ok // true表示允许用户输入自定义内容
    );
    if (ok) {
        qDebug() << "item is" << itemData;
    }
}

// 消息框示例（连续弹出多个对话框）
void MainWindow::on_pushButton_3_clicked()
{
    // 提问对话框（带Yes/No按钮）
    QMessageBox::StandardButton res = QMessageBox::question(
        this, tr("提问对话框"), tr("你玩原神吗?"),
        QMessageBox::Yes | QMessageBox::No
    );
    if (res == QMessageBox::Yes) {
        qDebug() << "yes";
    } else {
        return; // 用户选择了No，退出函数
    }

    // 信息对话框（仅显示信息，带Yes按钮）
    res = QMessageBox::information(
        this, tr("通知对话框"), tr("原神启动"),
        QMessageBox::Yes // 仅显示Yes按钮
    );
    if (res == QMessageBox::Yes) {
        qDebug() << "Standard yes";
    } else {
        return;
    }

    // 警告对话框（带Yes按钮）
    res = QMessageBox::warning(
        this, tr("警告对话框"), tr("不准启动"),
        QMessageBox::Yes
    );
    if (res == QMessageBox::Yes) {
        qDebug() << "warning yes";
    } else {
        return;
    }

    // 严重错误对话框（带Ok按钮）
    res = QMessageBox::critical(
        this, tr("关键提示对话框"), tr("肺雾熊猫仔"),
        QMessageBox::Ok
    );
    if (res == QMessageBox::Ok) {
        qDebug() << "critical Ok";
    } else {
        return;
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    _proDialog = new QProgressDialog(tr("正在复制"), tr("取消复制"), 0, 1000, this);
    // 2. 配置对话框属性
    _proDialog->setWindowTitle(tr("文件复制对话框"));  // 设置窗口标题
    _proDialog->setWindowModality(Qt::ApplicationModal);  // 设置模态（阻塞其他窗口操作）

    // 3. 创建定时器（用于模拟进度更新）
    _timer = new QTimer(this);  // 定时器依附于当前窗口，自动释放

    // 4. 连接信号与槽
    connect(_timer, &QTimer::timeout, this, &MainWindow::on_updateProDialog);  // 定时器超时信号触发进度更新
    connect(_proDialog, &QProgressDialog::canceled, this, &MainWindow::on_cancelProDialog);  // 取消按钮点击信号触发取消逻辑

    // 5. 启动定时器（每 2 毫秒触发一次超时信号）
    _timer->start(2);
}

void MainWindow::on_updateProDialog()
{
    _counter++;
    if (_counter > 1000) {
        _timer->stop();
        delete _timer;
        _timer = nullptr;
        delete _proDialog;
        _proDialog = nullptr;
        _counter = 0;
        return;
    }

    _proDialog->setValue(_counter);
}

void MainWindow::on_cancelProDialog()
{
    _timer->stop();
    delete _timer;
    _timer = nullptr;
    delete _proDialog;
    _proDialog = nullptr;
    _counter = 0;
    return;
}

void MainWindow::on_pushButton_5_clicked()
{
    // 使用智能指针管理内存，避免内存泄漏
    QScopedPointer<QWizard> wizard(new QWizard(this));
    wizard->setWindowTitle(tr("Man"));

    // 第一页：介绍信息
    QWizardPage *page1 = new QWizardPage(wizard.data()); // 设置父对象为wizard
    page1->setTitle(tr("What can I say"));

    QLabel *label1 = new QLabel(tr("ManBa Out!"), page1);
    QVBoxLayout *layout1 = new QVBoxLayout(page1);
    layout1->addWidget(label1);
    page1->setLayout(layout1);

    // 第二页：直升机类型选择
    QWizardPage *page2 = new QWizardPage(wizard.data());
    page2->setTitle(tr("选择直升机类型"));

    QButtonGroup *groupBtn = new QButtonGroup(page2);

    QRadioButton *btn1 = new QRadioButton(tr("黑鹰"), page2);
    QRadioButton *btn2 = new QRadioButton(tr("阿帕奇"), page2);
    QRadioButton *btn3 = new QRadioButton(tr("鱼鹰"), page2);
    QRadioButton *btn4 = new QRadioButton(tr("眼镜蛇"), page2);

    groupBtn->addButton(btn1);
    groupBtn->addButton(btn2);
    groupBtn->addButton(btn3);
    groupBtn->addButton(btn4);

    QVBoxLayout *layout2 = new QVBoxLayout(page2);
    layout2->addWidget(btn1);
    layout2->addWidget(btn2);
    layout2->addWidget(btn3);
    layout2->addWidget(btn4);
    page2->setLayout(layout2);

    // 正确的页面顺序：先添加第一页，再添加第二页
    wizard->addPage(page1);
    wizard->addPage(page2);

    // 显示向导对话框
    wizard->exec(); // 使用exec()显示模态对话框，用户完成后自动释放资源
}
```