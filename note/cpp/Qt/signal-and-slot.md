# connect函数详解 (Qt 5.11)
```cpp
[static] QMetaObject::Connection QObject::connect(const QObject *sender, const char *signal, const QObject *receiver, const char *method, Qt::ConnectionType type = Qt::AutoConnection)
```
创建连接，必须使用SIGNAL()和SLOT()宏标识信号与槽，例如

  QLabel *label = new QLabel;  
  QScrollBar *scrollBar = new QScrollBar;  
  QObject::connect(scrollBar, SIGNAL(valueChanged(int)),
                   label,  SLOT(setNum(int)));

注意，信号和槽中只能包含参数类型，不能包含参数名。下面是错的

  // WRONG  
  QObject::connect(scrollBar, SIGNAL(valueChanged(int value)),
                   label, SLOT(setNum(int value)));

一个信号能连接到另一个信号。  
一个信号可以连接到多个信号和多个槽，多个信号可以连接到一个槽。  
一个信号若连接到多个槽，信号发出时Qt将按照连接顺序依次调用槽函数。

若连接失败，函数返回一个非法的连接。若指定连接类型为Qt::UniqueConnection，则表示同一个“信号-槽”对不会出现两次，若重复则返回非法连接。  
参数类型影响连接的种类。它决定信号是否即刻被传递给槽，还是放入队列，后续发出。若信号需放入队列，那么参数类型必须在“元对象系统”内，自定义类型需要使用qRegisterMetaType()函数将类型注册到元对象系统。
```cpp
[static] QMetaObject::Connection QObject::connect(const QObject *sender, const QMetaMethod &signal, const QObject *receiver, const QMetaMethod &method, Qt::ConnectionType type = Qt::AutoConnection)
```
同上。
```cpp
QMetaObject::Connection QObject::connect(const QObject *sender, const char *signal, const char *method, Qt::ConnectionType type = Qt::AutoConnection) const
```
同 connect(sender, signal, this, method, type)
```cpp
[static] QMetaObject::Connection QObject::connect(const QObject *sender, PointerToMemberFunction signal, const QObject *receiver, PointerToMemberFunction method, Qt::ConnectionType type = Qt::AutoConnection)
```
connect()的重载，信号和槽函数都是基于函数的而非基于字符串，因此不需要使用SIGNAL和SLOT宏。
例子：

  QLabel *label = new QLabel;  
  QLineEdit *lineEdit = new QLineEdit;  
  QObject::connect(lineEdit, &QLineEdit::textChanged,
                   label,  &QLabel::setText);

自定义类型需要使用Q_DECLARE_METATYPE()宏将类型注册到元对象系统。
```cpp
[static] QMetaObject::Connection QObject::connect(const QObject *sender, PointerToMemberFunction signal, Functor functor)
```
将信号连接到槽函数、普通函数、成员函数或者lambda表达式。
若sender被销毁，则连接断开。
```cpp
[static] QMetaObject::Connection QObject::connect(const QObject *sender, PointerToMemberFunction signal, const QObject *context, Functor functor, Qt::ConnectionType type = Qt::AutoConnection)
```
若sender或context被销毁，则连接断开。

# 总结
connnect函数总共有上述六个，前三个是基于字符串的连接，后三个是基于函数的连接。主要不同点在于：  
基于字符串的连接在运行时检查类型、槽的参数可以多余信号（使用默认参数）、能将C++函数连接到QML函数；  
基于函数的连接在编译时检查类型、能进行隐式类型转换、能将信号连接到lambda表达式。  
特别地，如果槽函数是重载的，那么基于函数的连接需要指定重载函数类型，例子：
```cpp
auto slider = new QSlider(this);
auto lcd = new QLCDNumber(this);

// 基于字符串的语法
connect(slider, SIGNAL(valueChanged(int)),
        lcd, SLOT(display(int)));

// 基于函数的语法, 首选
connect(slider, &QSlider::valueChanged,
        lcd, static_cast<void (QLCDNumber::*)(int)>(&QLCDNumber::display));

// 基于函数的语法, 选择二
void (QLCDNumber::*mySlot)(int) = &QLCDNumber::display;
connect(slider, &QSlider::valueChanged,
        lcd, mySlot);

// 基于函数的语法, 选择三
connect(slider, &QSlider::valueChanged,
        lcd, QOverload<int>::of(&QLCDNumber::display));

// 基于函数的语法, 选择四 （需要C++14）
connect(slider, &QSlider::valueChanged,
        lcd, qOverload<int>(&QLCDNumber::display));
```
