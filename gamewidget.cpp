#include "gamewidget.h"
#include "ui_gamewidget.h"

GameWidget::GameWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);
    mysql->connect("",10);

}

//背景初始化
void GameWidget::setupScene(int i){

    gameOver=false;
    is_paused=false;
    exitMagic=true;

    score=0;
    trans=0;

    scoreTextLbl=new QLabel(this);
    scoreTextLbl->setGeometry(360,130,250,150);
    scoreTextLbl->setAlignment(Qt::AlignCenter);
    scoreTextLbl->setFont(QFont("Euclid",40,QFont::Normal));
    scoreTextLbl->setStyleSheet("QLabel{color:white;}");
    scoreTextLbl->setVisible(true);

    //禁用最大化按钮、设置窗口大小固定
    this->setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    this->setFixedSize(this->width(),this->height());
    //全屏
    QWidget::showFullScreen();
    //循环播放背景音乐

    sound=new QSoundEffect(this);
    sound->setSource(QUrl::fromLocalFile(QCoreApplication::applicationDirPath()+"/music/background/music-2.wav"));
    sound->setLoopCount(QSoundEffect::Infinite);
    sound->play();

    //窗口基本设置
    setWindowFlag(Qt::Window);  //设置为独立窗口
    setWindowTitle("Bejeweled");
    //setWindowIcon(QIcon("://picture/app_icon.ico"));

    //设置窗口背景黑色
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, Qt::black);
    this->setPalette(palette);

    //背景图片 棋盘框 分数框
    setAdaptedImg(":/picture/GameWidget/gamebk.png",ui->oriBkLbl);
    setAdaptedImg(":/picture/GameWidget/frame.png",ui->borderLbl);
    setAdaptedImg("",ui->scoreLbl);
    //设置变红的四周提示灯标签
    redBorder=new QLabel(this);
    redBorder->setGeometry(610, 2, 1055, 1073);
    setAdaptedImg(":/picture/frame_red.png",redBorder);
    redBorder->setAttribute(Qt::WA_TransparentForMouseEvents);
    //辅助label，不用管
    ui->menuLbl->setVisible(false);
    ui->hintLbl->setVisible(false);
    ui->pauseLbl->setVisible(false);
    ui->reSetBtn->setVisible(false);

    menuButton = new HoverButton(this);
    menuButton->setGeometry(ui->menuLbl->geometry());
    menuButton->setImage("",nullptr,ui->menuLbl->width(),ui->menuLbl->height(),ui->menuLbl);

    hintButton = new HoverButton(this);
    hintButton->setGeometry(ui->hintLbl->geometry());
    hintButton->setImage("",nullptr,ui->hintLbl->width(),ui->hintLbl->height(),ui->hintLbl);

    pauseButton = new HoverButton(this);
    pauseButton->setGeometry(ui->pauseLbl->geometry());
    pauseButton->setImage("",nullptr,ui->pauseLbl->width(),ui->pauseLbl->height(),ui->pauseLbl);

    reSetButton = new HoverButton(this);
    reSetButton->setGeometry(ui->reSetBtn->geometry());
    reSetButton->setImage("",nullptr,ui->reSetBtn->width(),ui->reSetBtn->height(),ui->reSetBtn);

    //语言切换
    if(i==1){
        menuButton->showContent("MENU",20);
        menuButton->show();
        hintButton->showContent("HINT",30);
        hintButton->show();
        pauseButton->showContent("PAUSE",20);
        pauseButton->show();
        reSetButton->showContent("RESET",15);
        reSetButton->show();
    }
    if(i==0){
        menuButton->showContent("菜单",20);
        menuButton->show();
        hintButton->showContent("提示",30);
        hintButton->show();
        pauseButton->showContent("暂停",20);
        pauseButton->show();
        reSetButton->showContent("重置",15);
        reSetButton->show();
    }
    // //设置鼠标-普通
    // setCursor(QCursor(QPixmap("://picture/mouse1.png")));

    //进度条
    int totalTime=8000;
    progressBar = new MyProBar(this);
    progressBar->setRange(0,totalTime);
    progressBar->setValue(totalTime);
    progressBar->setTextVisible(false);
    progressBar->show();

    //动画

    //棋盘
    QPropertyAnimation* anim1 = new QPropertyAnimation(ui->borderLbl, "geometry");
    anim1->setDuration(500);
    anim1->setStartValue(QRect(610+1055, 2, 1055, 1073));
    anim1->setEndValue(QRect(610, 2, 1055, 1073));
    anim1->setEasingCurve(QEasingCurve::OutQuad);
    //分数版
    QPropertyAnimation* anim2 = new QPropertyAnimation(ui->scoreLbl, "geometry");
    anim2->setDuration(500);
    anim2->setStartValue(QRect(270, 0, 327, 178));

    anim2->setEndValue(QRect(270, 80, 327, 178));
    anim2->setEasingCurve(QEasingCurve::InQuad);
    //进度条
    QPropertyAnimation* anim3 = new QPropertyAnimation(progressBar,"geometry");
    anim3->setDuration(500);
    anim3->setStartValue(QRect(653+1055,1010,982,47));
    anim3->setEndValue(QRect(653,1010,982,47));
    anim3->setEasingCurve(QEasingCurve::OutQuad);
    //菜单栏
    QPropertyAnimation* anim4 = new QPropertyAnimation(menuButton,"geometry");
    anim4->setDuration(500);
    anim4->setStartValue(QRect(menuButton->x(),menuButton->y()+1000,menuButton->width(),menuButton->height()));
    anim4->setEndValue(QRect(menuButton->x(),menuButton->y(),menuButton->width(),menuButton->height()));
    anim4->setEasingCurve((QEasingCurve::OutQuad));
    QPropertyAnimation* anim5 = new QPropertyAnimation(hintButton,"geometry");
    anim5->setDuration(500);
    anim5->setStartValue(QRect(hintButton->x(),hintButton->y()+1000,hintButton->width(),hintButton->height()));
    anim5->setEndValue(QRect(hintButton->x(),hintButton->y(),hintButton->width(),hintButton->height()));
    anim5->setEasingCurve((QEasingCurve::OutQuad));
    QPropertyAnimation* anim6 = new QPropertyAnimation(pauseButton,"geometry");
    anim6->setDuration(500);
    anim6->setStartValue(QRect(pauseButton->x(),pauseButton->y()+1000,pauseButton->width(),pauseButton->height()));
    anim6->setEndValue(QRect(pauseButton->x(),pauseButton->y(),pauseButton->width(),pauseButton->height()));
    anim6->setEasingCurve((QEasingCurve::OutQuad));
    QPropertyAnimation* anim7 = new QPropertyAnimation(reSetButton,"geometry");
    anim7->setDuration(500);
    anim7->setStartValue(QRect(reSetButton->x(),reSetButton->y()+1000,reSetButton->width(),reSetButton->height()));
    anim7->setEndValue(QRect(reSetButton->x(),reSetButton->y(),reSetButton->width(),reSetButton->height()));
    anim6->setEasingCurve((QEasingCurve::OutQuad));

    QParallelAnimationGroup *group = new QParallelAnimationGroup;
    group->addAnimation(anim1);
    group->addAnimation(anim2);
    group->addAnimation(anim3);
    group->addAnimation(anim4);
    group->addAnimation(anim5);
    group->addAnimation(anim6);
    group->addAnimation(anim7);
    group->start(QAbstractAnimation::DeleteWhenStopped);

    Sleep(600);
    startGame();

    //开始记时
    progressTimer = new QTimer(this);
    progressTimer->setInterval(15);
    progressTimer->start();
    redBorderTimer=new QTimer(this);
    redBorderTimer->setInterval(500);
    timeoutTimer=new QTimer(this);
    timeoutTimer->setInterval(30);
    //timeoutTimer->start();
    connect(redBorderTimer, &QTimer::timeout, [=](){
        if(redBordershow==0){
            redBorder->show();
            redBordershow=1;
        }else{
            redBorder->hide();
            redBordershow=0;
        }
    });
    //设置超时标签
    outLabel=new QLabel(this);
    connect(timeoutTimer, &QTimer::timeout, [=](){
        if(trans<=1)
            trans=trans+0.01;
        else {
            is_acting=false;
            return;
        }
        outLabel->setGeometry(837,388,600,300);

        outLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
        QPixmap input(":/picture/time_out.png");
        QImage image(input.size(), QImage::Format_ARGB32_Premultiplied);
        image.fill(Qt::transparent);
        QPainter p(&image);
        p.setOpacity(trans);
        p.drawPixmap(0, 0, input);
        p.end();
        QPixmap output = QPixmap::fromImage(image);
        outLabel->setPixmap(output);
        outLabel->show();
        outLabel->setParent(this);

    });

    connect(progressTimer, &QTimer::timeout, [=](){
        if(!is_paused){
            if(progressBar->value() == 0){
                gameOver=true;
                forbidAll(true);
                //allFallOut();

                //计时结束
                if(!timeoutTimer->isActive()){
                    timeoutTimer->start();
                    if(effect)
                        delete effect;
                    // effect=new QSound(":/music/effect/Time_Up.wav");
                    // effect->play();
                    //client->update(score);
                }
                if(redBorderTimer->isActive()){
                    redBorderTimer->stop();
                    redBorder->show();
                    redBordershow=1;
                }


            }
            else{
                progressBar->setValue(progressBar->value()-1);
                if(progressBar->value()/static_cast<double>(totalTime)<=0.25){
                    if(!redBorderTimer->isActive()){
                        redBorderTimer->start();
                    }

                }
            }
        }

    });

    connect(reSetButton,&HoverButton::clicked,[=]{
       if(is_acting||gameOver)
           return;
       reSetBoard();
    });

    //菜单按钮
    connect(menuButton, &HoverButton::clicked, [=](){

        if(is_acting)
            return;
        client->update(score);
        sound->stop();
        this->hide();
        showStartPage();

        if(timeoutTimer)
            delete timeoutTimer;
        if(outLabel)
            delete outLabel;
        if(redBorderTimer)
            delete redBorderTimer;
        if(redBorder)
            delete redBorder;

        if(reSetButton)
            delete reSetButton;
        if(menuButton)
            delete menuButton;

        if(hintButton)
            delete hintButton;
        if(pauseBKLbl)
            delete pauseBKLbl;
        if(pauseBKgif){
            pauseBKgif->stop();
            delete pauseBKgif;
        }
        if(pauseTXLbl)
            delete pauseTXLbl;
        if(pauseButton)
            delete pauseButton;
        if(progressTimer)
            delete progressTimer;
        if(progressBar)
            delete progressBar;
        if(selectedLbl)
            delete selectedLbl;
        if(scoreTextLbl)
            delete scoreTextLbl;

        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                delete gems[i][j];
            }
        }
        delete boardWidget;
    }) ;
    connect(hintButton, &HoverButton::clicked, [=](){     // 点击提示按钮的信号槽
        if(gameOver)  // 如果游戏已结束，不执行以下操作
            return;
        if(!is_acting && hintArrowTimes >= 6){  // 如果当前没有其他动作正在进行且提示箭头次数足够
            hintArrowTimes = 0;  // 重置提示箭头次数
            Point p = tipsdetect();  // 调用tipsdetect函数获取提示的位置
            QString msg = QTime::currentTime().toString() + " (" + QString::number(p.x) + "," + QString::number(p.y) + ")";
            QLabel *hintLabel = new QLabel(this);  // 创建用于显示箭头的标签
            hintLabel->setGeometry(665 + p.x * 118 + 39, 44 + p.y * 118 + 118, 40, 60);  // 设置箭头标签的位置和大小
            hintLabel->show();  // 显示箭头标签
            setAdaptedImg(":/picture/arrow.png", hintLabel);  // 设置箭头标签的图片

            // 创建箭头标签的出现动画
            QPropertyAnimation* anim = new QPropertyAnimation(hintLabel, "geometry");
            anim->setDuration(300);
            anim->setStartValue(QRect(hintLabel->x(), hintLabel->y() + 50, hintLabel->width(), hintLabel->height()));
            anim->setEndValue(QRect(hintLabel->x(), hintLabel->y(), hintLabel->width(), hintLabel->height()));
            anim->setEasingCurve(QEasingCurve::OutQuad);
            anim->start();  // 启动动画

            // 创建箭头标签的消失动画
            QPropertyAnimation* danim = new QPropertyAnimation(hintLabel, "geometry");
            connect(anim, &QPropertyAnimation::finished, [=]{
                danim->setDuration(300);
                danim->setEndValue(QRect(hintLabel->x(), hintLabel->y() + 50, hintLabel->width(), hintLabel->height()));
                danim->setStartValue(QRect(hintLabel->x(), hintLabel->y(), hintLabel->width(), hintLabel->height()));
                danim->setEasingCurve(QEasingCurve::InQuad);
                danim->start();  // 启动动画
            });

            connect(danim, &QPropertyAnimation::finished, [=]{
                hintArrowTimes = hintArrowTimes + 1;  // 提示箭头次数加1
                if(hintArrowTimes >= 6){  // 如果提示箭头次数达到上限
                    if(anim)
                        delete anim;
                    if(danim)
                        delete danim;
                    if(hintLabel)
                        delete hintLabel;  // 删除箭头标签
                }else{
                    anim->start();  // 重新启动箭头标签的出现动画
                }
            });
        }
    });


    connect(pauseButton,&HoverButton::clicked,[=]{
        if(gameOver)
            return;
        if(!is_acting){
            if(!is_paused)
            {
                if(i==1){
                    pauseButton->label.setText("CONTINUE");
                }else{
                    pauseButton->label.setText("继续");
                }
                forbidAll(true);
                pauseBKLbl = new QLabel(boardWidget);
                pauseBKLbl->setGeometry(0,0,952, 952);
                pauseBKgif = new QMovie("://picture/starsBK.gif",QByteArray(),boardWidget);
                pauseBKgif->setScaledSize(QSize(952,952));
                pauseBKLbl->setMovie(pauseBKgif);
                pauseBKLbl->show();
                pauseBKgif->start();

                pauseTXLbl = new QLabel(boardWidget);
                pauseTXLbl->setGeometry(250,boardWidget->height()/2,440,30);
                pauseTXLbl->setText("The Game Has Been Paused.");
                pauseTXLbl->setAlignment(Qt::AlignCenter);
                pauseTXLbl->setFont(QFont("BoDoni MT",25, QFont::Normal));
                pauseTXLbl->setStyleSheet("QLabel{color:white;}");
                pauseTXLbl->setVisible(true);
                is_paused=true;
            }
            else if(is_paused){
                if(i==1){
                    pauseButton->label.setText("PAUSE");
                }else{
                    pauseButton->label.setText("暂停");
                }
                pauseBKLbl->hide();
                if(pauseBKgif){
                    pauseBKgif->stop();
                }

                if(pauseBKLbl){

                }

                if(pauseTXLbl)
                    pauseTXLbl->setText("");

                forbidAll(false);
                is_paused=false;
            }
        }
    });
}

void GameWidget::reSetBoard() {
    // 执行所有宝石的掉落动画
    allFallOut();

    // 重新生成宝石类型
    QRandomGenerator::global()->fillRange(gemType[0], 64);

    // 创建并执行掉落动画的并行动画组
    QParallelAnimationGroup *group = new QParallelAnimationGroup;
    for(int j = 7; j >= 0; --j) {
        for(int i = 0; i < 8; ++i) {
            // 重新设置宝石类型，并创建新的宝石对象
            gemType[i][j] = gemType[i][j] % static_cast<unsigned int>(DIFFICULITY) + 1;
            gems[i][j] = new Gem(static_cast<int>(gemType[i][j]), 118, i, j, boardWidget);
            gems[i][j]->setAttribute(Qt::WA_TransparentForMouseEvents, true);

            // 将新宝石添加到掉落动画组
            group->addAnimation(startfallAnimation(gems[i][j], j + 1));

            // 连接新宝石的点击信号到槽函数act
            connect(gems[i][j], &Gem::mouseClickedGem, this, &GameWidget::act);
        }
    }

    // 启动掉落动画组
    group->start();

    // 重置计分和消除次数
    eliminateTimes = 0;

    // 连接掉落动画组完成信号到Lambda表达式
    connect(group, &QParallelAnimationGroup::finished, [=] {
        // 允许所有宝石的交互
        forbidAll(false);
        // 标记动作结束
        is_acting = false;

        // 更新炸弹列表，并加分
        int s = updateBombList();
        score += s;

        if(s != 0) {
            Sleep(100);
            // 禁用所有宝石的交互
            forbidAll(true);
            // 标记动作开始
            is_acting = true;
            // 执行宝石消除
            eliminateBoard();
            // 重置魔力状态
            exitMagic = false;
            // 增加消除次数
            eliminateTimes++;
        } else {
            eliminateTimes = 0;
        }

        // 播放消除音效
        playSound(eliminateTimes);

        // 释放动画组内存
        delete group;
    });
    return;
}

//将path的图片放置到label上，自适应label大小
void GameWidget::setAdaptedImg(QString path,QLabel *label)
{
    QImage image(path);
    QSize size=label->size();

    QImage image2=image.scaled(size,Qt::IgnoreAspectRatio);//重新调整图像大小以适应label
    label->setPixmap(QPixmap::fromImage(image2));//显示
}

void GameWidget::keyPressEvent(QKeyEvent *ev)
{
    //Esc退出全屏
    if(ev->key() == Qt::Key_Escape)
    {
        QWidget::showNormal();
        return;
    }
    //F11全屏
    if(ev->key() == Qt::Key_F11)
    {
        QWidget::showFullScreen();
        return;
    }
    QWidget::keyPressEvent(ev);
}

void GameWidget::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void GameWidget::forbidAll(bool forbid){//true forbit ,false release
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(gems[i][j])
                gems[i][j]->setAttribute(Qt::WA_TransparentForMouseEvents, forbid);
        }
    }
}

//关于宝石的随机数生成
int GameWidget::randomGem(){
    return QRandomGenerator::global()->bounded(1, DIFFICULITY+1);
}
void GameWidget::playSound(int type){
    QString src=":/music/effect/effect";
    switch(type){
    case 0:
        return;
    case 1:
        src.append("1.wav");
        break;
    case 2:
        src.append("2.wav");
        break;

    case 3:
        src.append("3.wav");
        break;

    case 4:
        src.append("4.wav");
        break;
    default:
        src.append("5.wav");
        break;
    }
    if(effect)
        delete effect;
    effect=new QSound(src);
    effect->play();
}

void GameWidget::startGame(){
    boardWidget = new QWidget(this);

    boardWidget->show();
    boardWidget->setGeometry(665, 44, 952, 952);

    QRandomGenerator::global()->fillRange(gemType[0], 64);

    //掉落动画
    QParallelAnimationGroup *group=new QParallelAnimationGroup;
    for(int j = 7; j >=0; --j){
        for(int i = 0; i <8 ; ++i){
            gemType[i][j] = gemType[i][j] % static_cast<unsigned int>(DIFFICULITY) + 1;
            gems[i][j] = new Gem(static_cast<int>(gemType[i][j]), 118, i, j , boardWidget);
            gems[i][j]->setAttribute(Qt::WA_TransparentForMouseEvents, true);
            group->addAnimation(startfallAnimation(gems[i][j],j+1));
            connect(gems[i][j], &Gem::mouseClickedGem, this, &GameWidget::act);
        }
    }
    group->start();
    eliminateTimes=0;

    connect(group, &QParallelAnimationGroup::finished, [=] {
        scoreTextLbl->setText("0");
        connect(this, &GameWidget::eliminateFinished, [=] {
            if(gameOver)
                return;
            Point p=tipsdetect();
            if(p.x==-1&&p.y==-1){
                Sleep(200);
                reSetBoard();
                return;
            }
            QSound *hit=new QSound(":/music/effect/hit.wav");
            hit->play();
            scoreTextLbl->setText(QString::number(score));
            forbidAll(false);
            is_acting=false;
            int s = updateBombList();
            score+=s;
            if(s!=0){
                Sleep(100);
                forbidAll(true);//禁用
                is_acting=true;
                eliminateBoard();
                eliminateTimes++;
                exitMagic=false;
            }else{
                eliminateTimes=0;
            }
            playSound(eliminateTimes);
        });
        forbidAll(false);
        is_acting=false;
        int s = updateBombList();
        score+=s;
        if(s!=0){

            Sleep(100);
            forbidAll(true);//禁用
            is_acting=true;
            eliminateBoard();
            exitMagic=false;
            eliminateTimes++;
        }else{
            eliminateTimes=0;
        }
        playSound(eliminateTimes);



        delete group;
    });
    connect(this,&GameWidget::finishCount,this,&GameWidget::finishAct);
}


void GameWidget::allFallOut(){
    is_acting=true;
    for(int j = 7; j >=0; --j){
        QParallelAnimationGroup* gruop = new QParallelAnimationGroup;
        for(int i = 0; i <8 ; ++i){
            QPropertyAnimation* anim = new QPropertyAnimation(gems[i][j],"geometry",boardWidget);
            anim->setDuration(700);
            anim->setStartValue(gems[i][j]->geometry());
            anim->setEndValue(QRect(gems[i][j]->geometry().x(),gems[i][j]->geometry().y()+1000,LEN,LEN));
            anim->setEasingCurve(QEasingCurve::InQuad);
            gruop->addAnimation(anim);
        }
        gruop->start(QAbstractAnimation::DeleteWhenStopped);
        Sleep(90);
    }
    Sleep(600);
//    for(int j = 7; j >=0; --j){
//        for(int i = 0; i <8 ; ++i){
//           gems[i][j]->bomb();
//        }
//    }
    is_acting=false;
}

void GameWidget::finishAct(){
    if(FTime==2){
        //当前页面宝石掉落
        magicFall();

        //随机生成新宝石并掉落
        magicFill();
        FTime=0;
    }
}

QPropertyAnimation* GameWidget::startfallAnimation(Gem *gem, int h){
    //每一行的掉落动画
    QPropertyAnimation* animation = new QPropertyAnimation(gem, "geometry", boardWidget);
    animation->setDuration(static_cast<int>((sqrt((8-h)*300+1050)*20)));//时间由高度决定
    animation->setStartValue(QRect(gem->oriX, gem->oriY-118*h-(8-h)*80, LEN, LEN));//高度由行数决定
    animation->setEndValue(QRect(gem->oriX, gem->oriY, LEN, LEN));
    animation->setEasingCurve(QEasingCurve::Linear);
    return animation;
}

void GameWidget::magicCollect(int coType, int toX, int toY) {
    // 创建一个列表以存储相同类型的宝石
    std::vector<Gem*> list;

    // 创建一个并行动画组
    QParallelAnimationGroup* group = new QParallelAnimationGroup;

    // 遍历宝石网格（8x8），查找指定类型的宝石
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(gemType[i][j] == static_cast<unsigned int>(coType)) {
                list.push_back(gems[i][j]); // 将相同类型的宝石添加到列表中
            }
        }
    }

    // 为列表中的每个宝石创建动画
    for(Gem* gem : list) {
        QPropertyAnimation* anim = new QPropertyAnimation(gem, "geometry", boardWidget);
        anim->setDuration(500);
        anim->setStartValue(QRect(gem->geometry()));
        anim->setEndValue(QRect(gems[toX][toY]->geometry()));
        anim->setEasingCurve(QEasingCurve::OutQuad);
        group->addAnimation(anim);
    }

    // 将目标位置的宝石加入列表
    list.push_back(gems[toX][toY]);
    group->start();

    // 连接动画组完成信号，进行后续处理
    connect(group, &QParallelAnimationGroup::finished, [=]{
        delete group;

        // 重置宝石落下高度数组
        for(int i = 0; i < 8; ++i)
            for(int j = 0; j < 8; ++j)
                fallHeight[i][j] = 0;

        // 计算当前页面宝石要掉落的高度
        for(unsigned int i = 0; i < list.size(); i++) {
            gemType[list[i]->x][list[i]->y] = 100;
            fallHeight[list[i]->x][list[i]->y] = -1;
            for(int k = list[i]->y - 1; k >= 0; --k)
                if(fallHeight[list[i]->x][k] != -1)
                    fallHeight[list[i]->x][k]++;
        }

        // 触发每个宝石的爆炸效果
        for(Gem* gem : list) {
            gem->bomb();
        }
        Sleep(200);

        // 执行宝石下落和填充
        fall();
        fill();
    });
}

//处理游戏中宝石的点击、交换、旋转等操作
void GameWidget::act(Gem* gem){
    hintArrowTimes = 6;  // 提示箭头次数初始化为6

    int len = 118;  // 宝石之间的间距
    int gemX = gem->x;  // 宝石的横坐标
    int gemY = gem->y;  // 宝石的纵坐标

    // 如果当前没有宝石被选中，则让点击的宝石选中
    if(selectedX == -1 && selectedY == -1){
        selectedX = gemX;
        selectedY = gemY;
        // 加选框
        selectedLbl = new QLabel(boardWidget);
        QImage image("://picture/GameWidget/selected.png");
        selectedLbl->setPixmap(QPixmap::fromImage(image));
        selectedLbl->setGeometry(len * selectedX, len * selectedY, len, len);
        selectedLbl->setAttribute(Qt::WA_TransparentForMouseEvents);
        selectedLbl->show();
        // 让选中宝石旋转
        if(gems[selectedX][selectedY]->type != 0){
            makeSpin(selectedX, selectedY);
        }
    }
    // 如果有宝石选中，并点击了邻居宝石，则让宝石交换
    else if(((selectedX == gemX) && (abs(selectedY - gemY) == 1))
             || ((selectedY == gemY) && (abs(selectedX - gemX) == 1))){
        int SX = selectedX;
        int SY = selectedY;
        selectedX = -1;
        selectedY = -1;
        if(gems[SX][SY]->type != 0)
            makeStopSpin(SX, SY);
        else{
            makeSpin(SX, SY);
        }
        // 去选框
        selectedLbl->clear();

        gems[gemX][gemY]->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        gems[SX][SY]->setAttribute(Qt::WA_TransparentForMouseEvents, true);

        // 让选中宝石与(gemX, gemY)交换
        swap(SX, SY, gemX, gemY);
        Sleep(300);

        // 在对象矩阵中交换
        std::swap(gems[gemX][gemY], gems[SX][SY]);
        std::swap(gems[gemX][gemY]->x, gems[SX][SY]->x);
        std::swap(gems[gemX][gemY]->y, gems[SX][SY]->y);
        std::swap(gemType[gemX][gemY], gemType[SX][SY]);

        if(gems[SX][SY]->type == 0){
            magicCollect(gems[gemX][gemY]->type, SX, SY);
        }
        else if(gems[gemX][gemY]->type == 0){
            magicCollect(gems[SX][SY]->type, gemX, gemY);
        }
        else{
            int currentScore = updateBombList();  // 将这次的分数返回，如果是0就回退
            score += currentScore;
            if(currentScore == 0) {
                if(effect)
                    delete effect;
                effect = new QSound(":/music/effect/bad.wav");
                effect->play();
                std::swap(gemX, SX);
                std::swap(gemY, SY);

                swap(SX, SY, gemX, gemY);
                Sleep(500);

                // 在对象矩阵中交换
                std::swap(gems[gemX][gemY], gems[SX][SY]);
                std::swap(gems[gemX][gemY]->x, gems[SX][SY]->x);
                std::swap(gems[gemX][gemY]->y, gems[SX][SY]->y);
                std::swap(gemType[gemX][gemY], gemType[SX][SY]);
                is_acting = false;

                gems[gemX][gemY]->setAttribute(Qt::WA_TransparentForMouseEvents, false);
                gems[SX][SY]->setAttribute(Qt::WA_TransparentForMouseEvents, false);
            }
            else{
                forbidAll(true);  // 禁用
                is_acting = true;
                gems[gemX][gemY]->setAttribute(Qt::WA_TransparentForMouseEvents, false);
                gems[SX][SY]->setAttribute(Qt::WA_TransparentForMouseEvents, false);
                eliminateTimes = 1;
                playSound(eliminateTimes);
                if(exitMagic){
                    eliminateBoard();
                } else{
                    bombList.clear();
                    int magicType1 = getBombsToMakeMagic(SX, SY, bombsToMakeMagic1, 1);

                    generateMagic(SX, SY, magicType1, 1);
                    int magicType2 = getBombsToMakeMagic(gemX, gemY, bombsToMakeMagic2, 2);

                    generateMagic(gemX, gemY, magicType2, 2);
                }

            }
            exitMagic = false;
        }
    }
    // 如果点击了自己
    else if((selectedX == gemX) && (selectedY == gemY)){

        // 去选框
        selectedLbl->clear();
        // 静止
        if(gems[selectedX][selectedY]->type != 0)
            makeStopSpin(selectedX, selectedY);

        selectedX = -1;
        selectedY = -1;
    }else{
        // 去选框
        selectedLbl->clear();
        // 静止
        if(gems[selectedX][selectedY]->type != 0)
            makeStopSpin(selectedX, selectedY);
        selectedX = gemX;
        selectedY = gemY;
        // 加选框
        selectedLbl = new QLabel(boardWidget);
        QImage image("://picture/GameWidget/selected.png");
        selectedLbl->setPixmap(QPixmap::fromImage(image));
        selectedLbl->setGeometry(len * selectedX, len * selectedY, len, len);
        selectedLbl->setAttribute(Qt::WA_TransparentForMouseEvents);
        selectedLbl->show();
        // 让选中宝石旋转
        makeSpin(selectedX, selectedY);
    }
}

//交换按钮位置
void GameWidget::swap(int SX,int SY,int gemX,int gemY){
    //处理宝石交换
    int xVal1 = gems[SX][SY]->x*118;int yVal1 = gems[SX][SY]->y*118;
    int width1 = gems[SX][SY]->width();int height1 = gems[SX][SY]->height();

    int xVal2 = gems[gemX][gemY]->x*118;int yVal2 = gems[gemX][gemY]->y*118;
    int width2 = gems[gemX][gemY]->width();int height2 = gems[gemX][gemY]->height();

    //宝石
    QParallelAnimationGroup* group = new QParallelAnimationGroup;
    QPropertyAnimation *anim1 = new QPropertyAnimation(gems[SX][SY],"geometry",boardWidget);
    anim1->setDuration(300);
    anim1->setStartValue(QRect(xVal1,yVal1,width1,height1));
    anim1->setEndValue(QRect(xVal2,yVal2,width2,height2));
    anim1->setEasingCurve(QEasingCurve::Linear);
    //宝石动图
    QPropertyAnimation *anim11 = new QPropertyAnimation(gems[SX][SY]->spinLabel,"geometry",boardWidget);
    anim11->setDuration(300);
    anim11->setStartValue(QRect(xVal1,yVal1,width1,height1));
    anim11->setEndValue(QRect(xVal2,yVal2,width2,height2));
    anim11->setEasingCurve(QEasingCurve::Linear);
    //被交换宝石
    QPropertyAnimation *anim2 = new QPropertyAnimation(gems[gemX][gemY],"geometry",boardWidget);
    anim2->setDuration(300);
    anim2->setStartValue(QRect(xVal2,yVal2,width2,height2));
    anim2->setEndValue(QRect(xVal1,yVal1,width1,height1));
    anim2->setEasingCurve(QEasingCurve::Linear);

    group->addAnimation(anim1);
    group->addAnimation(anim11);
    group->addAnimation(anim2);
    group->start(QAbstractAnimation::DeleteWhenStopped);

    mysql->change(client->username,score);
    mysql->add(client->username,score);


}

//下落动画
void GameWidget::fallAnimation(Gem *gem, int h){
    gem->setAttribute(Qt::WA_TransparentForMouseEvents, true); // 屏蔽鼠标，防止交互
    QPropertyAnimation* animation = new QPropertyAnimation(gem, "geometry", this); // 创建属性动画对象，作用于宝石的几何属性
    animation->setDuration(static_cast<int>(sqrt(h * 118 * 900))); // 设置动画持续时间，这里使用了一个基于高度的平方根函数，可根据实际情况调整
    animation->setStartValue(gem->geometry()); // 设置动画起始值为当前宝石的几何属性
    animation->setEndValue(QRect(gem->geometry().x(), gem->geometry().y() + LEN * h, gem->width(), gem->height())); // 设置动画结束值为宝石下落后的位置
    animation->setEasingCurve(QEasingCurve::InQuad); // 设置缓动曲线，这里使用了二次缓动
    animation->start(); // 启动动画

    connect(animation, &QPropertyAnimation::finished, [=]{
        fallCount++; // 下落完成计数加一
        gem->setAttribute(Qt::WA_TransparentForMouseEvents, false); // 取消屏蔽鼠标，允许后续交互
        delete animation; // 动画完成后释放动画对象
    });
}

//填充动画
void GameWidget::fillfallAnimation(Gem *gem, int h){
    gem->setAttribute(Qt::WA_TransparentForMouseEvents, true); // 屏蔽鼠标，防止交互
    QPropertyAnimation* animation = new QPropertyAnimation(gem, "geometry", this); // 创建属性动画对象，作用于宝石的几何属性
    animation->setDuration(static_cast<int>(sqrt(h * 118 * 900))); // 设置动画持续时间，这里使用了一个基于高度的平方根函数，可根据实际情况调整
    animation->setStartValue(gem->geometry()); // 设置动画起始值为当前宝石的几何属性
    animation->setEndValue(QRect(gem->geometry().x(), gem->geometry().y() + LEN * h, gem->width(), gem->height())); // 设置动画结束值为宝石下落后的位置
    animation->setEasingCurve(QEasingCurve::InQuad); // 设置缓动曲线，这里使用了二次缓动
    animation->start(); // 启动动画

    connect(animation, &QPropertyAnimation::finished, [=] {
        fallCount++; // 下落完成计数加一
        if(fallCount == fallNum){ // 如果下落完成的计数等于总的下落数量
            is_acting = false; // 填充完成，不再处于动作状态
            eliminateFinished(); // 发射信号，表示填充完成
        }
        gem->setAttribute(Qt::WA_TransparentForMouseEvents, false); // 取消屏蔽鼠标，允许后续交互
        delete animation; // 动画完成后释放动画对象
    });
}

//实现下落
void GameWidget::fall(){
    for(int i = 0; i < 8; ++i)
        for(int j = 7; j >= 0; --j){
            if(fallHeight[i][j] != -1 && fallHeight[i][j] != 0 && gemType[i][j] != 100){
                // 更新宝石类型和位置
                gemType[i][j + fallHeight[i][j]] = gemType[i][j];
                gems[i][j]->setY(gems[i][j]->y + fallHeight[i][j]);
                gems[i][j + fallHeight[i][j]] = gems[i][j];
                gemType[i][j] = 100; // 100表示该位置上没有宝石

                // 执行宝石下落动画
                fallAnimation(gems[i][j], fallHeight[i][j]);
            }
        }
}

//特殊下落
void GameWidget::magicFall(){
    for(int i = 0; i < 8; ++i)
        for(int j = 7; j >= 0; --j){
            if(tHeight[i][j] != -1 && tHeight[i][j] != 0 && gemType[i][j] != 100){
                // 更新宝石类型和位置
                gemType[i][j + tHeight[i][j]] = gemType[i][j];
                gems[i][j]->setY(gems[i][j]->y + tHeight[i][j]);
                gems[i][j + tHeight[i][j]] = gems[i][j];
                gemType[i][j] = 100; // 100表示该位置上没有宝石

                // 执行宝石下落动画
                fallAnimation(gems[i][j], tHeight[i][j]);
            }
        }
}

//特殊填充
void GameWidget::magicFill(){
    QTimer::singleShot(100, this, [=](){
        int lack[8] = {0}; // 用于记录每列需要填充的数量

        // 计算每列需要填充的数量
        for(int i = 0; i < 8; ++i){
            for(int j = 0; j < 8; ++j)
                if(tHeight[i][j] == -1)
                    lack[i]++;
                else if(tHeight[i][j] != 0){
                    lack[i] += tHeight[i][j];
                    break;
                }
        }

        // 根据需要填充的数量创建新的宝石，并连接点击事件的槽函数
        for(int i = 0; i < 8; ++i)
            for(int j = 0; j < lack[i]; ++j){
                gems[i][lack[i]-j-1] = new Gem(randomGem(), LEN, i, lack[i]-j-1, boardWidget, -lack[i]);
                gems[i][lack[i]-j-1]->setGeometry(LEN*i, LEN*(-j-1), LEN, LEN);
                gemType[i][lack[i]-j-1] = static_cast<unsigned int>(gems[i][lack[i]-j-1]->type);
                connect(gems[i][lack[i]-j-1], &Gem::mouseClickedGem, this, &GameWidget::act);
            }

        // 统计需要下落的宝石数量
        for(int i = 0; i < 8; ++i)
            for(int j = 7; j >= 0; --j){
                if(tHeight[i][j] != -1 && tHeight[i][j] != 0 && gemType[i][j] != 100){
                    fallNum++;
                }
            }
        for(int i = 0; i < 8; ++i)
            fallNum += lack[i];

        // 执行宝石填充的动画
        for(int i = 0; i < 8; ++i)
            for(int j = 0; j < lack[i]; ++j){
                fillfallAnimation(gems[i][lack[i]-j-1], lack[i]);
            }

    });
}

//实现填充
void GameWidget::fill(){
    QTimer::singleShot(100, this, [=](){
        int lack[8] = {0};

        for(int i = 0; i < 8; ++i){
            for(int j = 0; j < 8; ++j)
                if(fallHeight[i][j] == -1)//这一列全消的情况
                    lack[i]++;
                else if(fallHeight[i][j] != 0){
                    lack[i] += fallHeight[i][j];//这一列不是全消的情况，直接利用fallHeight即可
                    break;
                }
        }

        for(int i = 0; i < 8; ++i)
            for(int j = 0; j < lack[i]; ++j){
                gems[i][lack[i]-j-1] = new Gem(randomGem(), LEN, i, lack[i]-j-1, boardWidget, -lack[i]);
                gems[i][lack[i]-j-1]->setGeometry(LEN*i, LEN*(-j-1), LEN, LEN);
                gemType[i][lack[i]-j-1] = static_cast<unsigned int>(gems[i][lack[i]-j-1]->type);
                connect(gems[i][lack[i]-j-1], &Gem::mouseClickedGem, this, &GameWidget::act);
            }

        for(int i = 0; i < 8; ++i)
            for(int j = 7; j >= 0; --j){
                if(fallHeight[i][j] != -1 && fallHeight[i][j] != 0 && gemType[i][j] != 100){
                    fallNum++;
                }
            }

        for(int i = 0; i < 8; ++i)
            fallNum+=lack[i];

        for(int i = 0; i < 8; ++i)
            for(int j = 0; j < lack[i]; ++j){
                fillfallAnimation(gems[i][lack[i]-j-1], lack[i]);
            }

    });
}

//生成Magic,并处理
void GameWidget::generateMagic(int cX, int cY, int type, int time){
    std::vector<Gem*> tempList; // 用于存储动画用的宝石指针

    // 处理第一次点击（time == 1）
    if(time == 1){
        if(type == -1)
            bombsToMakeMagic1.clear();
        if(type == 0)
            bombsToMakeMagic1.push_back(gems[cX][cY]);

        // 将需要进行特殊处理的宝石加入 tempList
        for(unsigned int i = 0; i < bombsToMakeMagic1.size(); i++){
            tempList.push_back(bombsToMakeMagic1[i]);
        }
    }
    // 处理第二次点击（time == 2）
    else if(time == 2){
        if(type == -1)
            bombsToMakeMagic2.clear();
        if(type == 0)
            bombsToMakeMagic2.push_back(gems[cX][cY]);

        // 将需要进行特殊处理的宝石加入 tempList
        for(unsigned int i = 0; i < bombsToMakeMagic2.size(); i++){
            tempList.push_back(bombsToMakeMagic2[i]);
        }

        std::vector<Gem*> combine;
        for(unsigned int i = 0; i < bombsToMakeMagic1.size(); i++){
            combine.push_back(bombsToMakeMagic1[i]);
        }
        for(unsigned int i = 0; i < bombsToMakeMagic2.size(); i++){
            combine.push_back(bombsToMakeMagic2[i]);
        }
        bombsToMakeMagic1.clear();
        bombsToMakeMagic2.clear();

        for(int i = 0; i < 8; i++)
            for(int j = 0; j < 8; j++){
                tHeight[i][j] = 0;
            }

        // 计算当前页面宝石要掉落的高度
        for(unsigned int i = 0; i < combine.size(); i++){
            gemType[combine[i]->x][combine[i]->y] = 100;
            tHeight[combine[i]->x][combine[i]->y] = -1;
            for(int k = combine[i]->y - 1; k >= 0; k--)
                if(tHeight[combine[i]->x][k] != -1)
                    tHeight[combine[i]->x][k]++;
        }
    }

    // 处理特殊的消除效果
    if(type == -1){
        // 魔法效果结束，清零计数
        fallNum = fallCount = 0;
        FTime++;
        finishCount();
        return;
    }

    if(type == 0){
        // 普通三消动画
        int r = 10;

        QParallelAnimationGroup* biggerGroup = new QParallelAnimationGroup;
        for(unsigned int i = 0; i < tempList.size(); i++){
            QPropertyAnimation *anim = new QPropertyAnimation(tempList[i], "geometry", boardWidget);
            anim->setDuration(250);
            anim->setStartValue(QRect(tempList[i]->geometry()));
            anim->setEndValue(QRect(tempList[i]->geometry().x() - r, tempList[i]->geometry().y() - r, LEN + 2 * r, LEN + 2 * r));
            anim->setEasingCurve(QEasingCurve::OutQuad);
            biggerGroup->addAnimation(anim);
        }
        biggerGroup->start();

        connect(biggerGroup, &QParallelAnimationGroup::finished, [=] {
            delete biggerGroup;
            QParallelAnimationGroup* smallerGroup = new QParallelAnimationGroup;
            for(unsigned int i = 0; i < tempList.size(); i++){
                QPropertyAnimation *anim = new QPropertyAnimation(tempList[i], "geometry", boardWidget);
                anim->setDuration(200);
                anim->setStartValue(QRect(tempList[i]->geometry()));
                anim->setEndValue(QRect(tempList[i]->geometry().x() + r + LEN / 2 - 1, tempList[i]->geometry().y() + r + LEN / 2 - 1, 2, 2));
                anim->setEasingCurve(QEasingCurve::InQuad);
                smallerGroup->addAnimation(anim);
            }
            smallerGroup->start();
            connect(smallerGroup, &QParallelAnimationGroup::finished, [=] {
                delete smallerGroup;
                for(unsigned int i = 0; i < tempList.size(); i++)
                    if(tempList[i])
                        tempList[i]->bomb();
                fallNum = fallCount = 0;
                FTime++;
                finishCount();
            });
        });
    }
    else if(type != -1){
        // 特殊消除，周围的宝石聚集到中心动画
        QParallelAnimationGroup* gather = new QParallelAnimationGroup;
        for(unsigned int i = 0; i < tempList.size(); i++){
            QPropertyAnimation* anim = new QPropertyAnimation(tempList[i], "geometry", boardWidget);
            anim->setDuration(280);
            anim->setStartValue(tempList[i]->geometry());
            anim->setEndValue(gems[cX][cY]->geometry());
            anim->setEasingCurve(QEasingCurve::Linear);
            gather->addAnimation(anim);
        }
        gather->start();

        connect(gather, &QParallelAnimationGroup::finished, [=] {
            delete gather;

            // 将周围的宝石消除
            for(unsigned int i = 0; i < tempList.size(); i++)
                if(tempList[i])
                    tempList[i]->bomb();

            unsigned int tty = gemType[cX][cY] * 10 + static_cast<unsigned int>(type);

            if(tty % 10 == 3){ // 魔方
                gemType[cX][cY] = tty;
                gems[cX][cY]->type = 0;
                makeStopSpin(cX, cY);
                makeSpin(cX, cY);
            }
            else if(tty % 10 == 2){ // 爆炸
                gemType[cX][cY] = tty;
                gems[cX][cY]->type = static_cast<int>(tty);
                int idx = tty % 10 + 10; // 12

                // 原图
                gems[cX][cY]->normalLabel = new QLabel(gems[cX][cY]);
                gems[cX][cY]->normalLabel->setGeometry(0, 0, gems[cX][cY]->width(), gems[cX][cY]->height());
                setAdaptedImg(gems[cX][cY]->path_stable[(tty - tty % 10) / 10], gems[cX][cY]->normalLabel);
                gems[cX][cY]->normalLabel->show();
                // 特效图
                gems[cX][cY]->setStyleSheet(QString("QPushButton{background-color:transparent;border:0px;}"));
                gems[cX][cY]->magicLabel = new QLabel(gems[cX][cY]);
                gems[cX][cY]->magicLabel->setGeometry(0, 0, gems[cX][cY]->width(), gems[cX][cY]->height());
                gems[cX][cY]->magicGif = new QMovie(gems[cX][cY]->path_dynamic[idx], QByteArray(), gems[cX][cY]);
                gems[cX][cY]->magicGif->setScaledSize(QSize(gems[cX][cY]->width(), gems[cX][cY]->height()));
                gems[cX][cY]->magicLabel->setMovie(gems[cX][cY]->magicGif);
                gems[cX][cY]->magicLabel->show();
                gems[cX][cY]->magicGif->start();
            }
            else if(tty % 10 == 1){ // 十字消
                gemType[cX][cY] = tty;
                gems[cX][cY]->type = static_cast<int>(tty);
                int idx = tty % 10 + 10; // 11

                // 原图
                gems[cX][cY]->normalLabel = new QLabel(gems[cX][cY]);
                gems[cX][cY]->normalLabel->setGeometry(0, 0, gems[cX][cY]->width(), gems[cX][cY]->height());
                setAdaptedImg(gems[cX][cY]->path_stable[(tty - tty % 10) / 10], gems[cX][cY]->normalLabel);
                gems[cX][cY]->normalLabel->show();
                // 特效图
                gems[cX][cY]->setStyleSheet(QString("QPushButton{background-color:transparent;border:0px;}"));
                gems[cX][cY]->magicLabel = new QLabel(gems[cX][cY]);
                gems[cX][cY]->magicLabel->setGeometry(0, 0, gems[cX][cY]->width(), gems[cX][cY]->height());
                gems[cX][cY]->magicGif = new QMovie(gems[cX][cY]->path_dynamic[idx], QByteArray(), gems[cX][cY]);
                gems[cX][cY]->magicGif->setScaledSize(QSize(gems[cX][cY]->width(), gems[cX][cY]->height()));
                gems[cX][cY]->magicLabel->setMovie(gems[cX][cY]->magicGif);
                gems[cX][cY]->magicLabel->show();
                gems[cX][cY]->magicGif->start();
            }

            Sleep(100); // 延时效果

            fallNum = fallCount = 0;
            FTime++;
            finishCount();
        });
    }
}


int GameWidget::getBombsToMakeMagic(int cX, int cY, std::vector<Gem*> bombsToMakeMagic, int time){
    int rNum = 0, cNum = 0; // 同行、同列相同个数

    // 检测以(cX, cY)为起点，同行连着的有几个（除去自己）
    // 1. 向右检测
    int start = cX, end = cX + 1;
    while(end <= 7 && gemType[start][cY] == gemType[end][cY]){
        bombsToMakeMagic.push_back(gems[end][cY]);
        end++;
    }
    end--;
    rNum += end - start;
    // 2. 向左检测
    start = cX; end = cX - 1;
    while(end >= 0 && gemType[start][cY] == gemType[end][cY]){
        bombsToMakeMagic.push_back(gems[end][cY]);
        end--;
    }
    end++;
    rNum += start - end;

    if(rNum == 1){
        bombsToMakeMagic.pop_back();
    }

    // 检测同列连着的有几个（除去自己）
    // 1. 向下检测
    start = cY; end = cY + 1;
    while(end <= 7 && gemType[cX][start] == gemType[cX][end]){
        bombsToMakeMagic.push_back(gems[cX][end]);
        end++;
    }
    end--;
    cNum += end - start;
    // 2. 向上检测
    start = cY; end = cY - 1;
    while(end >= 0 && gemType[cX][start] == gemType[cX][end]){
        bombsToMakeMagic.push_back(gems[cX][end]);
        end--;
    }
    end++;
    cNum += start - end;

    if(cNum == 1){
        bombsToMakeMagic.pop_back();
    }

    int magicType = -1; // -1 无特殊效果, 0 普通三消, 1 爆炸, 2 十字消, 3 魔方
    if(bombsToMakeMagic.size() >= 3){
        if(cNum == 4 || rNum == 4)
            magicType = 3; // 魔方
        else if((rNum == 3 && cNum == 0) || (rNum == 0 && cNum == 3))
            magicType = 2; // 十字消
        else
            magicType = 1; // 爆炸
    } else if(bombsToMakeMagic.size() == 2){
        if((rNum == 2 && cNum <= 1) || (rNum <= 1 && cNum == 2))
            magicType = 0; // 普通三消
    }

    // 将检测得到的需要特殊处理的宝石存储在 bombsToMakeMagic1 或 bombsToMakeMagic2 中
    if(time == 1){
        bombsToMakeMagic1 = bombsToMakeMagic;
    } else if(time == 2){
        bombsToMakeMagic2 = bombsToMakeMagic;
    }

    return magicType;
}


//downConvert 1,2,3...7
unsigned int dC(unsigned int t){
    return (t-t%10)/10;
}

//更新分数及bomblist列表
int GameWidget::updateBombList() {
    if(gameOver)
        return 0;

    int score = 0;  // 初始化分数，每消除一个宝石加10分

    unsigned int eliminating[8][8];  // 用于标记要消除的宝石坐标
    memset(eliminating, 0, sizeof(eliminating));

    // 水平方向检测
    for (int i = 0; i < 8; i++) {
        int start = 0, end = 0;
        while (start <= 5) {
            unsigned int sType, eType;

            // 获取起点和终点的宝石类型
            if (gemType[i][start] > 10)
                sType = dC(gemType[i][start]);
            else
                sType = gemType[i][start];

            if (gemType[i][end] > 10)
                eType = dC(gemType[i][end]);
            else
                eType = gemType[i][end];

            // 找出连续相同类型的宝石
            while (sType == eType) {
                end++;
                if (gemType[i][end] > 10)
                    eType = dC(gemType[i][end]);
                else
                    eType = gemType[i][end];
            }
            end--;

            // 标记要消除的区域
            if (end - start >= 2) {
                for (int j = start; j <= end; j++) {
                    if (gemType[i][j] > 10) {
                        exitMagic = true;

                        // 根据特殊宝石类型标记周围区域
                        if (gemType[i][j] % 10 == 2) {  // 爆炸-菱形区域
                            for (int m = -1; m <= 1; m++) {
                                for (int n = -1; n <= 1; n++) {
                                    if (i + m >= 0 && i + m <= 7 && j + n >= 0 && j + n <= 7)
                                        eliminating[i + m][j + n] = gemType[i + m][j + n];
                                }
                            }
                            if (j - 2 >= 0)
                                eliminating[i][j - 2] = gemType[i][j - 2];
                            if (j + 2 <= 7)
                                eliminating[i][j + 2] = gemType[i][j + 2];
                            if (i - 2 >= 0)
                                eliminating[i - 2][j] = gemType[i - 2][j];
                            if (i + 2 <= 7)
                                eliminating[i + 2][j] = gemType[i + 2][j];
                        }
                        if (gemType[i][j] % 10 == 1) {  // 十字消-十字区域
                            for (int m = 0; m <= 7; m++)
                                eliminating[m][j] = gemType[m][j];
                            for (int n = 0; n <= 7; n++)
                                eliminating[i][n] = gemType[i][n];
                        }
                    } else {
                        eliminating[i][j] = gemType[i][j];
                    }
                }
            }
            end++;
            start = end;
        }
    }

    // 垂直方向检测
    for (int i = 0; i < 8; i++) {
        int start = 0, end = 0;
        while (start <= 5) {
            unsigned int sType, eType;

            // 获取起点和终点的宝石类型
            if (gemType[start][i] > 10)
                sType = dC(gemType[start][i]);
            else
                sType = gemType[start][i];

            if (gemType[end][i] > 10)
                eType = dC(gemType[end][i]);
            else
                eType = gemType[end][i];

            // 找出连续相同类型的宝石
            while (sType == eType) {
                end++;
                if (gemType[end][i] > 10)
                    eType = dC(gemType[end][i]);
                else
                    eType = gemType[end][i];
            }
            end--;

            // 标记要消除的区域
            if (end - start >= 2) {
                for (int j = start; j <= end; j++) {
                    if (gemType[j][i] > 10) {
                        exitMagic = true;

                        // 根据特殊宝石类型标记周围区域
                        if (gemType[j][i] % 10 == 2) {  // 爆炸
                            for (int m = -1; m <= 1; m++) {
                                for (int n = -1; n <= 1; n++) {
                                    if (j + m >= 0 && j + m <= 7 && i + n >= 0 && i + n <= 7)
                                        eliminating[j + m][i + n] = gemType[j + m][i + n];
                                }
                            }
                            if (j - 2 >= 0)
                                eliminating[j - 2][i] = gemType[j - 2][i];
                            if (j + 2 <= 7)
                                eliminating[j + 2][i] = gemType[j + 2][i];
                            if (i - 2 >= 0)
                                eliminating[j][i - 2] = gemType[j][i - 2];
                            if (i + 2 <= 7)
                                eliminating[j][i + 2] = gemType[j][i + 2];
                        }
                        if (gemType[j][i] % 10 == 1) {  // 十字消
                            for (int m = 0; m <= 7; m++)
                                eliminating[j][m] = gemType[j][m];
                            for (int n = 0; n <= 7; n++)
                                eliminating[n][i] = gemType[n][i];
                        }
                    } else {
                        eliminating[j][i] = gemType[j][i];
                    }
                }
            }
            end++;
            start = end;
        }
    }

    // 遍历 eliminating 数组，生成 bombList 列表，计算分数
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (eliminating[i][j] != 0 && eliminating[i][j] != 100) {
                bombList.push_back(gems[i][j]);
                score += 10;  // 每个加10分
            }
        }
    }


    // if(!mysql->userinfo.contains(client->username)){
    //     qDebug()<<"no contain";
    //     mysql->add(client->username,score);

    // }else{
    //     qDebug()<<"do contain";
    //     mysql->change(client->username,score);

    // }

    // mysql->add(client->username,score);
    // mysql->change(client->username,score);
    return score;
}


//提示功能
Point GameWidget::tipsdetect(){
    int types[12][12];
    for (int i = 0; i < 10; ++i) {
        types[0][i]=-1;
        types[1][i]=-1;
        types[10][i]=-1;
        types[11][i]=-1;
        types[i][0]=-1;
        types[i][1]=-1;
        types[i][10]=-1;
        types[i][11]=-1;
    }
    for(int i=2;i<=9;i++){
        for(int j=2;j<=9;j++){
            types[i][j]=gems[i-2][j-2]->type;
        }
    }
    //分两种情况讨论
    for(int i=2;i<=9;i++){
        for (int j=2;j<=9;j++) {
            int t=types[i][j];
            //第一，检测连着两个相同类型的宝石，并检测其周围
            if(t==types[i][j+1]){
                if(t==types[i][j-2])
                    return Point(i-2,j-4);
                if(t==types[i-1][j-1])
                    return Point(i-3,j-3);
                if(t==types[i+1][j-1])
                    return Point(i-1,j-3);
                if(t==types[i-1][j+2])
                    return Point(i-3,j);
                if(t==types[i+1][j+2])
                    return Point(i-1,j);
                if(t==types[i][j+3])
                    return Point(i-2,j+1);

            }
            //第二，检测两个跳着的宝石，看是否有宝石能填补中间位置
            if(t==types[i][j+2]){
                if(t==types[i-1][j+1])
                    return Point(i-3,j-1);
                if(t==types[i+1][j+1])
                    return Point(i-1,j-1);
            }
        }
    }
    //列检测
    for(int i=2;i<=9;i++){
        for (int j=2;j<=9;j++) {
            int t=types[j][i];
            if(t==types[j+1][i]){
                if(t==types[j-2][i])
                    return Point(j-4,i-2);
                if(t==types[j-1][i-1])
                    return Point(j-3,i-3);
                if(t==types[j-1][i+1])
                    return Point(j-3,i-1);
                if(t==types[j+2][i-1])
                    return Point(j,i-3);
                if(t==types[j+2][i+1])
                    return Point(j,i-1);
                if(t==types[j+3][i])
                    return Point(j+1,i-2);

            }
            if(t==types[j+2][i]){
                if(t==types[j+1][i-1])
                    return Point(j-1,i-3);
                if(t==types[j+1][i+1])
                    return Point(j-1,i-1);

            }
        }
    }

    return Point(-1,-1);
}

//处理消除和掉落
void GameWidget::eliminateBoard() {
    // 如果游戏结束，则退出函数
    if (gameOver)
        return;

    // 标记当前正在执行动作
    is_acting = true;

    // 如果没有可消除的宝石，则退出函数
    if (bombList.size() == 0)
        return;

    // 初始化宝石掉落高度数组
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            fallHeight[i][j] = 0;

    // 计算当前页面宝石要掉落的高度
    for (unsigned int i = 0; i < bombList.size(); i++) {
        gemType[bombList[i]->x][bombList[i]->y] = 100;
        fallHeight[bombList[i]->x][bombList[i]->y] = -1;
        for (int k = bombList[i]->y - 1; k >= 0; --k)
            if (fallHeight[bombList[i]->x][k] != -1)
                fallHeight[bombList[i]->x][k]++;
    }

    // 消去宝石并执行放大效果
    int r = 10;
    for (unsigned int i = 0; i < bombList.size(); i++) {
        if (!bombList[i])
            bombList.erase(bombList.begin() + i);
    }
    QParallelAnimationGroup* biggerGroup = new QParallelAnimationGroup;
    for (unsigned int i = 0; i < bombList.size(); i++) {
        QPropertyAnimation *anim = new QPropertyAnimation(bombList[i], "geometry", boardWidget);
        anim->setDuration(250);
        anim->setStartValue(QRect(bombList[i]->geometry()));
        anim->setEndValue(QRect(bombList[i]->geometry().x() - r, bombList[i]->geometry().y() - r, LEN + 2 * r, LEN + 2 * r));
        anim->setEasingCurve(QEasingCurve::OutQuad);
        biggerGroup->addAnimation(anim);
    }
    biggerGroup->start();
    connect(biggerGroup, &QParallelAnimationGroup::finished, [=] {
        delete biggerGroup;
        QParallelAnimationGroup* smallerGroup = new QParallelAnimationGroup;

        // 执行缩小效果
        for (unsigned int i = 0; i < bombList.size(); i++) {
            QPropertyAnimation *anim = new QPropertyAnimation(bombList[i], "geometry", boardWidget);
            anim->setDuration(150);
            anim->setStartValue(QRect(bombList[i]->geometry()));
            anim->setEndValue(QRect(bombList[i]->geometry().x() + r + LEN / 2 - 1,
                                    bombList[i]->geometry().y() + r + LEN / 2 - 1, 2, 2));
            anim->setEasingCurve(QEasingCurve::InQuad);
            smallerGroup->addAnimation(anim);
        }
        smallerGroup->start();
        connect(smallerGroup, &QParallelAnimationGroup::finished, [=] {
            delete smallerGroup;

            // 消去宝石
            for (unsigned int i = 0; i < bombList.size(); i++)
                if (bombList[i])
                    bombList[i]->bomb();

            // 清空待消除宝石列表
            bombList.clear();

            // 重置掉落计数
            fallNum = fallCount = 0;

            // 当前页面宝石掉落
            fall();

            // 随机生成新宝石并掉落
            fill();
        });
    });
}

//旋转
void GameWidget::makeSpin(int SX,int SY){
    if(!gems[SX][SY]||SX==-1)
        return;
    gems[SX][SY]->setStyleSheet(QString("QPushButton{background-color:transparent;border:0px;}"));
    gems[SX][SY]->spinLabel = new QLabel(gems[SX][SY]);
    gems[SX][SY]->spinLabel->setGeometry(0,0,gems[SX][SY]->width(), gems[SX][SY]->height());
    gems[SX][SY]->spinGif = new QMovie(gems[SX][SY]->path_dynamic[gems[SX][SY]->type], QByteArray(), gems[SX][SY]);
    gems[SX][SY]->spinGif->setScaledSize(QSize(gems[SX][SY]->width(), gems[SX][SY]->height()));
    gems[SX][SY]->spinLabel->setMovie(gems[SX][SY]->spinGif);
    gems[SX][SY]->spinLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    gems[SX][SY]->spinLabel->show();
    gems[SX][SY]->spinGif->start();
}

//停止旋转
void GameWidget::makeStopSpin(int SX,int SY){
    if(!gems[SX][SY]||SX==-1)
        return;
    if(gems[SX][SY]->spinGif)
        gems[SX][SY]->spinGif->stop();
    if(gems[SX][SY]->spinLabel)
        gems[SX][SY]->spinLabel->clear();

    gems[SX][SY]->setStyleSheet(QString("QPushButton{border-image:url(%1);}").arg(gems[SX][SY]->path_stable[gems[SX][SY]->type]));
    gems[SX][SY]->setIconSize(QSize(LEN, LEN));
}
