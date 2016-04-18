# Test task.

## Fixed requirements for the code:

* Interface for packet-receiver should be:
```
struct IReceiver
{
	virtual void Receive(const char* data, unsigned int size) = 0;
};
```

* Interface for packet-callback should be:
```
struct ICallback
{
	virtual void BinaryPacket(const char* data, unsigned int size) = 0;
	virtual void TextPacket(const char* data, unsigned int size) = 0;
};
```