1. std::mutex

```c++
std::mutex mtx;

{
  mtx.lock();
  // critical code
  mtx.unlock();
}

{
  if(mtx.try_lock()){
    // critical code
    mtx.unlock()
  }
}
```

2. std::lock_guard std::unique_lock

```c++
std::mutex mtx;

{
  std::lock_guard<std::mutex> lg(mtx);
  // critical code
}

{
  {
    std::unique_lock<std::mutex> ul(mtx);
    // critical code
    // attention: unique_lock has the following functions: lock try_lock lock_for try_lock lock_util unlock
  }
}
```

3. std::condition_variable

```c++
std::mutex mtx;

{
  std::condition_variable cv;
  cv.wait(ul,_predicate);
}
```
implement:
template<typename _Predicate>
void wait(unique_lock<mutex>& __lock, _Predicate __p) {
  while (!__p())  wait(__lock);
}

