# tool_bluetooth_socket_connector
A small C++ client/server tool for information transfer over IP/Bluetooth Socket. 
The develop environment is Ubuntu 14.04 64bit.

#Environment setup
cmake         (sudo apt-get install cmake)
libbluetooth  (sudo apt-get install libbluetooth-dev:i386)
libpthread
librt

#Build
```sh
./build.sh
```

#Run the demo
```sh
usage: Build/BluetoothSocketConnectors/Test/Demo/BluetoothSocketConnectors_Test_Demo <0:HU|1:SP> <0:Use IP Socket|1:Use BT Socket>

[Terminal 1 for socket server]
Build/BluetoothSocketConnectors/Test/Demo/BluetoothSocketConnectors_Test_Demo 0 0
[Terminal 2 for socket client]
Build/BluetoothSocketConnectors/Test/Demo/BluetoothSocketConnectors_Test_Demo 1 0
```

#Other Infomation
  None.
  
