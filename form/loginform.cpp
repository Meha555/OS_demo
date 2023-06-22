#include "loginform.h"
#include "ui_loginform.h"

LoginForm::LoginForm(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::SplashScreen); //设置为SplashScreen, 窗口无边框,不在任务栏显示
    ui->btnOpenDB->setRole(Material::Primary);
    connect(ui->btnOpenDB,&QPushButton::clicked,this,[this](){
        dbName = QFileDialog::getOpenFileName(this, tr("选择Sqlite数据库文件")
                                                               , ""
                                                               , tr("SQLite db (*.db *.sqlite)"));
        if(dbName.isEmpty()){
            QMessageBox::warning(this,"错误","打开数据库文件失败!");
            this->reject();
        }
        else{
            dbName.replace("/", "//");
            this->accept();
        }
    });
    QMovie* movie = new QMovie(":/icons/assets/icons/loading.gif");
    ui->label->setMovie(movie);
    movie->start();
    ui->label->show();
}

LoginForm::~LoginForm()
{
    delete ui;
}

QString LoginForm::getDbName() const
{
    return dbName;
}

void LoginForm::setDbName(const QString &value)
{
    dbName = value;
}

void LoginForm::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_moving = true;
        //记录下鼠标相对于窗口的位置
        //event->globalPos()鼠标按下时，鼠标相对于整个屏幕位置
        //pos() this->pos()鼠标按下时，窗口相对于整个屏幕位置
        m_lastPos = event->globalPos() - pos();
    }
    return QDialog::mousePressEvent(event);
}

void LoginForm::mouseMoveEvent(QMouseEvent *event)
{
    //通过事件event->globalPos()知道鼠标坐标，鼠标坐标减去鼠标相对于窗口位置，就是窗口在整个屏幕的坐标
    if (m_moving && (event->buttons() & Qt::LeftButton)
        && (event->globalPos()-m_lastPos).manhattanLength() > QApplication::startDragDistance())
    {
        move(event->globalPos()-m_lastPos);
        m_lastPos = event->globalPos() - pos();
    }
    return QDialog::mouseMoveEvent(event);
}

void LoginForm::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_moving=false; //停止移动
}
