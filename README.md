# sosan-server
This is a server that is implemented in C/C++. <br />
We support Linux first.

# Overall
![Alt text](overall.jpg?raw=true)

# Server Internal
![Alt text](server_internal.jpg?raw=true)

# Abstract Plugin Handler Interface
```
class PluginHandler {

  MemPool mp; // Memory pool object

  virtual bool init() = 0;

  virtual bool request() = 0;

  virtual bool destroy() = 0;

  virtual bool heartbeat() = 0;
};
```
