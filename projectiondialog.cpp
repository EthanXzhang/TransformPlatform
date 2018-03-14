#include "projectiondialog.h"
#include "ui_projectiondialog.h"

ProjectionDialog::ProjectionDialog(QWidget *parent,MovieInfo *p) :
    QDialog(parent),
    ui(new Ui::ProjectionDialog)
{
    ui->setupUi(this);
    movieinfo=p;
    initUI();
    initComboBox();
    getMovieInfo();
    setAttribute(Qt::WA_DeleteOnClose);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(setMovieInfo()));
}

ProjectionDialog::~ProjectionDialog()
{
    delete ui;
}
void ProjectionDialog::initUI()
{
    algorithmbox=ui->algorithmBox;
    inputformatbox=ui->inputformatBox;
    outputformatbox=ui->outputformatBox;
    enablelowpassbox=ui->enableBox;
    enableMTbox=ui->mtBox;
    widthline=ui->widthEdit;
    heightline=ui->heightEdit;
    cubelenghtline=ui->lenghtEdit;
    maxcubelenghtline=ui->maxlengthEdit;
    vsline=ui->vsEdit;
    hsline=ui->hsEdit;
}
void ProjectionDialog::initComboBox()
{
    QString projectionformat[]={"CUBEMAP 32","CUBEMAP 23 OFFCENTER","EQUIRECT","BARREL","EAC32"};
    for(int i=0;i<5;i++)
    {
        inputformatbox->addItem(projectionformat[i]);
        outputformatbox->addItem(projectionformat[i]);
    }
    inputformatbox->setCurrentIndex(2);
    QString algortithm[]={"CUBIC","NEAREST","LINEAR","LANCZOS4"};
    for(int i=0;i<4;i++)
    {
        algorithmbox->addItem(algortithm[i]);
    }
}
void ProjectionDialog::getMovieInfo()
{
    if(movieinfo==NULL)
    {
        return ;
    }
    //获取输入投影格式
    switch (movieinfo->setting.input_layout) {
    case LAYOUT_CUBEMAP_32:
        inputformatbox->setCurrentIndex(0);
        break;
    case LAYOUT_CUBEMAP_23_OFFCENTER:
        inputformatbox->setCurrentIndex(1);
        break;
    case LAYOUT_EQUIRECT:
        inputformatbox->setCurrentIndex(2);
        break;
    case LAYOUT_BARREL:
        inputformatbox->setCurrentIndex(3);
        break;
    case LAYOUT_EAC_32:
        inputformatbox->setCurrentIndex(4);
        break;
    default:
        break;
    }
    //获取输出投影格式
    switch (movieinfo->setting.output_layout) {
    case LAYOUT_CUBEMAP_32:
        outputformatbox->setCurrentIndex(0);
        break;
    case LAYOUT_CUBEMAP_23_OFFCENTER:
        outputformatbox->setCurrentIndex(1);
        break;
    case LAYOUT_EQUIRECT:
        outputformatbox->setCurrentIndex(2);
        break;
    case LAYOUT_BARREL:
        outputformatbox->setCurrentIndex(3);
        break;
    case LAYOUT_EAC_32:
        outputformatbox->setCurrentIndex(4);
        break;
    default:
        break;
    }
    //获取插值算法
    switch (movieinfo->setting.interpolation_alg) {
    case CUBIC:
        algorithmbox->setCurrentIndex(0);
        break;
    case NEAREST:
        algorithmbox->setCurrentIndex(1);
        break;
    case LINEAR:
        algorithmbox->setCurrentIndex(2);
        break;
    case LANCZOS4:
        algorithmbox->setCurrentIndex(3);
        break;
    default:
        break;
    }
    //获取低通滤波配置
    if(movieinfo->setting.enable_low_pass_filter)
    {
        enablelowpassbox->setCheckState((Qt::CheckState)2);
    }
    else
    {
        enablelowpassbox->setCheckState((Qt::CheckState)0);
    }
    if(movieinfo->setting.enable_multi_threading)
    {
        enableMTbox->setCheckState((Qt::CheckState)2);
    }
    else
    {
        enableMTbox->setCheckState((Qt::CheckState)0);
    }
    vsline->setText(QString::number(movieinfo->setting.num_vertical_segments));
    hsline->setText(QString::number(movieinfo->setting.num_horizontal_segments));
    //获取输出分辨率设定
    if(movieinfo->setting.w==0)
    {
        widthline->setText("Auto");
    }
    else
    {
        widthline->setText(QString::number(movieinfo->setting.w));
    }
    if(movieinfo->setting.h==0)
    {
        heightline->setText("Auto");
    }
    else
    {
        heightline->setText(QString::number(movieinfo->setting.h));
    }
    //获取立方块尺寸设定
    if(movieinfo->setting.cube_edge_length==0)
    {
        cubelenghtline->setText("Auto");
    }
    else
    {
        cubelenghtline->setText(QString::number(movieinfo->setting.cube_edge_length));
    }
    maxcubelenghtline->setText(QString::number(movieinfo->setting.max_cube_edge_length));
}
void ProjectionDialog::setMovieInfo()
{
    //设置分辨率
    if(QString::compare(widthline->text(),"Auto")!=0)
    {
        movieinfo->setting.w=widthline->text().toInt();

    }
    if(QString::compare(heightline->text(),"Auto")!=0)
    {
        movieinfo->setting.h=heightline->text().toInt();
    }
    //设置输入投影格式
    switch(inputformatbox->currentIndex())
    {
    case 0:
        movieinfo->setting.input_layout=LAYOUT_CUBEMAP_32;
        break;
    case 1:
        movieinfo->setting.input_layout=LAYOUT_CUBEMAP_23_OFFCENTER;
        break;
    case 2:
        movieinfo->setting.input_layout=LAYOUT_EQUIRECT;
        break;
    case 3:
        movieinfo->setting.input_layout=LAYOUT_BARREL;
        break;
    case 4:
        movieinfo->setting.input_layout=LAYOUT_EAC_32;
        break;
    }
    switch(outputformatbox->currentIndex())
    {
    case 0:
        movieinfo->setting.output_layout=LAYOUT_CUBEMAP_32;
        break;
    case 1:
        movieinfo->setting.output_layout=LAYOUT_CUBEMAP_23_OFFCENTER;
        break;
    case 2:
        movieinfo->setting.output_layout=LAYOUT_EQUIRECT;
        break;
    case 3:
        movieinfo->setting.output_layout=LAYOUT_BARREL;
        break;
    case 4:
        movieinfo->setting.output_layout=LAYOUT_EAC_32;
        break;
    }
    //设置插值算法
    switch (algorithmbox->currentIndex())
    {
    case 0:
        movieinfo->setting.interpolation_alg=CUBIC;
        break;
    case 1:
        movieinfo->setting.interpolation_alg=NEAREST;
        break;
    case 2:
        movieinfo->setting.interpolation_alg=LINEAR;
        break;
    case 3:
        movieinfo->setting.interpolation_alg=LANCZOS4;
        break;
    default:
        break;
    }
    //设置立方块尺寸
    if(QString::compare(cubelenghtline->text(),"Auto")!=0)
    {
        movieinfo->setting.cube_edge_length=cubelenghtline->text().toInt();
    }
    movieinfo->setting.max_cube_edge_length=maxcubelenghtline->text().toInt();
    //设置滤波器
    movieinfo->setting.enable_low_pass_filter=enablelowpassbox->isChecked();
    movieinfo->setting.enable_multi_threading=enableMTbox->isChecked();
    movieinfo->setting.num_vertical_segments=vsline->text().toInt();
    movieinfo->setting.num_horizontal_segments=hsline->text().toInt();
}
