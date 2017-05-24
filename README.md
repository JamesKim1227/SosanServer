# sosan-server
This is a server that is implemented in Java, Netty Framework. <br />
This project is in the process of development.

# Overall
![Alt text](overall.jpg?raw=true)

# Server Internal
![Alt text](server_internal.jpg?raw=true)

# Abstract Plugin Handler Interface
```java
public interface AbstractPluginHandler {
  boolean init();

  boolean request();

  boolean destroy();

  boolean heartBeat();
}

class PluginHandler implements AbstractPluginHandler {
  ...
}
```
