# QObject
## Thread Affinity
A QObject instance is said to have a thread affinity, or that it lives in a certain thread. When a QObject receives a queued signal or a posted event, the slot or event handler will run in the thread that the object lives in.

Note: If a QObject has no thread affinity (that is, if thread() returns zero), or if it lives in a thread that has no running event loop, then it cannot receive queued signals or posted events.

By default, a QObject lives in the thread in which it is created. An object's thread affinity can be queried using thread() and changed using moveToThread().

All QObjects must live in the same thread as their parent. Consequently:

> setParent() will fail if the two QObjects involved live in different threads.  
When a QObject is moved to another thread, all its children will be automatically moved too.  
> moveToThread() will fail if the QObject has a parent.  
> If QObjects are created within QThread::run(), they cannot become children of the QThread object because the QThread does not live in the thread that calls QThread::run().  

Note: A QObject's member variables do not automatically become its children. The parent-child relationship must be set by either passing a pointer to the child's constructor, or by calling setParent(). Without this step, the object's member variables will remain in the old thread when moveToThread() is called.

## Connection
```cpp
static QMetaObject::Connection connect(const QObject *sender, const char *signal, const QObject *receiver, const char *member, Qt::ConnectionType = Qt::AutoConnection);
```
* a direct connection means that the slot is always invoked directly by the thread the signal is emitted from;
* a queued connection means that an event is posted in the event queue of the thread the receiver is living in, which will be picked up by the event loop and will cause the slot invocation sometime later;
* a blocking queued connection is like a queued connection, but the sender thread blocks until the event is picked up by the event loop of the thread the receiver is living in, the slot is invoked, and it returns;
* an automatic connection (the default) means that if the thread the receiver is living in is the same as the current thread, a direct connection is used; otherwise, a queued connection is used.

### Note
若连接类型为AutoConnection，则连接的类型不是由信号发送者所依附的线程决定，而是由发送信号(emit)所在线程和信号接收者所依附的线程是否相同决定：若相同则为DirectConnection，槽函数立即被调用；否则为QueuedConnection，槽函数加入接受者所依附线程的事件队列，等待被调用。  
例如，信号函数在主线程被发射，槽函数依附于工作线程，若工作线程没有单独的事件循环，则该槽函数不会被触发。
