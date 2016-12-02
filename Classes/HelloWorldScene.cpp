#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;


struct Balloon {
    int tag;
    std::string image;
    Color3B color;
    Color3B labelColor;
} YellowBallon,BlueBallon,GreenBallon;



Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    //Color4B(255, 255, 255, 255)	135-206-250
    auto bg = cocos2d::LayerColor::create(Color4B(135, 206, 250, 255));
    this->addChild(bg);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    YellowBallon ={ .tag = 101, .color = Color3B::YELLOW,.labelColor = Color3B::BLUE,.image = "YellowBalloon.png" };
    BlueBallon ={ .tag = 102, .color = Color3B::BLUE,.labelColor = Color3B::WHITE ,.image = "BlueBalloon.png" };
    GreenBallon ={ .tag = 103, .color = Color3B::GREEN,.labelColor = Color3B::RED ,.image = "GreenBalloon.png"  };
    
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("balloon-burst.wav");
    //->playEffect("balloon-burst.wav");

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    closeItem->setScale(3.0f);
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width*3/2 ,
                                origin.y + closeItem->getContentSize().height*3/2));

    // create menu, it's an autorelease object
    closeItem->setTag(201);
    auto menu = Menu::create(closeItem, NULL);

    menu->setPosition(Vec2::ZERO);

    menu->setTag(200);

    //menu->setAnchorPoint(Vec2::ZERO);
    this->addChild(menu,1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Burst the Balloon", "fonts/Marker Felt.ttf", 24);
    label->setColor(Color3B::YELLOW);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 0);

    // add "HelloWorld" splash screen"
    //auto sprite  = [CCSprite spriteWithFile:@"HelloWorld.png" rect:CGRectMake(0, 0, 152, 152)];
    auto sprite = Sprite::create("Sisters.png");
    sprite->setTag(100);
    
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("res/explosion.plist");
    
    
    auto yellowBalloon = Sprite::create(YellowBallon.image);
    cache->addSpriteFrame(yellowBalloon->getSpriteFrame(), YellowBallon.image);
    auto blueBalloon = Sprite::create(BlueBallon.image);
    cache->addSpriteFrame(blueBalloon->getSpriteFrame(), BlueBallon.image);
    auto greenBalloon = Sprite::create(GreenBallon.image);
    cache->addSpriteFrame(greenBalloon->getSpriteFrame(), GreenBallon.image);

    //[self addChild:sprite];
    
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->onTouchBegan = [&](Touch* touch, Event* event){
//        Size visibleSize = Director::getInstance()->getVisibleSize();
//        for (auto& child : this->getChildren()){
//            if (child->getBoundingBox().containsPoint(this->convertTouchToNodeSpace(touch)) == true
//                && child->getTag() == 100) {
//                child->setPosition(Vec2(child->getPosition().x+visibleSize.width/10, child->getPosition().y));
//                log("touched %f",visibleSize.width);
//            }
//        }
//        return false;
//        
//    };
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    // add the sprite as a child to this layer
    this->addChild(sprite);
	this->schedule(schedule_selector(HelloWorld::updateGame));    
    return true;
}

void HelloWorld::updateGame(float dt)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    for (auto& child : this->getChildren()){
        if(child->getTag() > 100 && child->getTag() < 200){
            if(child->getPosition().x>visibleSize.width+child->getContentSize().width){
               //child->setPosition(Vec2(0, child->getPosition().y));
                this->removeChild(child);
            }else{
               child->setPosition(Vec2(child->getPosition().x+visibleSize.width*(child->getTag() -100)/1000, child->getPosition().y));
            }

        }
    }
    Vector<Node*> myVector = getChildren();
    
    if(myVector.size()<9){
        this->addBalloon();
    }
}

void HelloWorld::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event)
{
    Touch* touch = (Touch*)(touches.at(0));
    Size visibleSize = Director::getInstance()->getVisibleSize();
    for (auto& child : this->getChildren()){
        log("touched %d",child->getTag());
        if (child->getBoundingBox().containsPoint(this->convertTouchToNodeSpace(touch)) == true) {

            if(child->getTag() > 100 && child->getTag() < 200){
                child->setPosition(Vec2(child->getPosition().x+visibleSize.width/10, child->getPosition().y));
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("balloon-burst.wav");
//                auto cache = AnimationCache::getInstance();
//                cache->addAnimationsWithFile("res/explosion.plist");
//                auto animation = cache->getAnimation("explosion");
//                auto animate = Animate::create(animation);
//                child->runAction(animate);
                Vector<SpriteFrame*> *animFrames = new Vector<SpriteFrame *>;;
                SpriteFrameCache* cache = SpriteFrameCache::getInstance();
                
                GLboolean isLoaded = cache->isSpriteFramesWithFileLoaded("res/explosion.plist");
                //log("isLoaded %d",isLoaded);
                if(!isLoaded){
                    cache->addSpriteFramesWithFile("res/explosion.plist");
                }
//                auto frame1 = Sprite::createWithSpriteFrameName("Explotion1.png");
//                auto frame2 = Sprite::createWithSpriteFrameName("Explotion2.png");
//                auto frame3 = Sprite::createWithSpriteFrameName("Explotion3.png");
//                animFrames->pushBack(frame1->getSpriteFrame());
//                animFrames->pushBack(frame2->getSpriteFrame());
//                animFrames->pushBack(frame3->getSpriteFrame());
                
                animFrames->pushBack(cache->getSpriteFrameByName("Explotion1.png"));
                animFrames->pushBack(cache->getSpriteFrameByName("Explotion2.png"));
                animFrames->pushBack(cache->getSpriteFrameByName("Explotion3.png"));
                auto animation = Animation::createWithSpriteFrames(*animFrames, 0.2f);
                auto animate = Animate::create(animation);
                //child->runAction(animate);
                
 
               // auto callbackAction = CC_CALLBACK_1(HelloWorld::explosionCallback, this);
                //CallFuncN::create(CC_CALLBACK_1(HelloWorld::explosionCallback,this));
                

                
                Sprite* s = dynamic_cast<Sprite*>(child);
                if(s != 0) {
                    for (auto& sChild : s->getChildren()){
                        sChild->setVisible(false);
                    }
                    FiniteTimeAction *callAct = CallFunc::create(CC_CALLBACK_0(HelloWorld::explosionCallback, this,s));
                    child->runAction(Sequence::create(animate, callAct, nullptr));
                }
                
            }
        }
    }
}

void HelloWorld::explosionCallback(Sprite* s)
{
    log("explosionCallback...");
    for (auto& sChild : s->getChildren()){
        Label* label = dynamic_cast<Label*>(sChild);
        auto tmpStr = label->getString();
        log("You Pressed %s",tmpStr.c_str());
        sChild->setVisible(true);
    }
    this->removeChild(s);
}

Sprite* HelloWorld::addBalloon()
{
    log("generateBalloon...");
    int balloonType = RandomHelper::random_int(101, 103);
    
    Balloon tmpBalloon;
    switch (balloonType) {
        case 101:
            tmpBalloon = YellowBallon;
            break;
        case 102:
            tmpBalloon = BlueBallon;
            break;
        case 103:
            tmpBalloon = GreenBallon;
            break;
        default:
            tmpBalloon = GreenBallon;
            break;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto ballonSprite = Sprite::create(tmpBalloon.image);
    auto randomY = RandomHelper::random_real(origin.y, visibleSize.height);
    ballonSprite->setPosition(Vec2(origin.x, randomY));
    
    char label = RandomHelper::random_int(65, 90);
    std::string labelStr = std::string(1 , label );
    auto balloonLabel = Label::createWithTTF(labelStr, "fonts/Marker Felt.ttf", 24);
    
    balloonLabel->setColor(tmpBalloon.labelColor);
    ballonSprite->addChild(balloonLabel,1);
    balloonLabel->setPosition(20, 40);
    //balloonLabel->setTextColor(tmpBalloon.labelColor);

    
    this->addChild(ballonSprite);
    ballonSprite->setTag(balloonType);
    return ballonSprite;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    log("Over...");
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
