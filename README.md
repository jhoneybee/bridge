# bridge 

功能支持如下

- 打印Word文件
- 编辑Word文件
- 预览Word文件


## Web调用例子


```js
const socket = new WebSocket('ws://localhost:29580');

// 如果已经关闭连接,则进行
if (socket.readyState === socket.CLOSED) {
    alert("未启动插件程序,请点击安装后进行启动.")
}

function preview(){
    socket.send(JSON.stringify({
        "channel": "OFFICE_OPEN_WORD_PREVIEW",
        "params": {
            "target": "http://127.0.0.1:8080/demo.docs"
        }
    }))
}

function print(){
    socket.send(JSON.stringify({
        "channel": "OFFICE_OPEN_WORD_EDITOR",
        "params": {
            "target": "http://127.0.0.1:8080/demo.docs"
        }
    }))
}

function editor(){
    socket.send(JSON.stringify({
        "channel": "OFFICE_OPEN_WORD_EDITOR",
        "params": {
            "target": "http://127.0.0.1:8080/1.docs",
            "saveUrl": "http://127.0.0.1:8080/saveUrl",
            "filename": "xxxx.docs"
        }
    }))
}
```

### 预览 WORD/EXCEL/PPT


```json
{
    "channel": "OFFICE_OPEN_${TYPE}_PREVIEW",
    "params": {
        "target": "http://127.0.0.1:8080/demo.docs"
    }
}
```


| 名称 | 类型 | 描述
|----  |----  |------
|target | 字符串类型 | 要访问的文档地址，可以是一个本地的文档，也可以是一个url地址



### 打印 WORD/EXCEL/PPT

```json
{
    "channel": "OFFICE_OPEN_${type}_EDITOR",
    "params": {
        "target": "http://127.0.0.1:8080/demo.docs"
    }
}
```

| 名称 | 类型 | 描述
|----  |----  |------
|target | 字符串类型 | 要访问的文档地址，可以是一个本地的文档，也可以是一个url地址


### 编辑 WORD/EXCEL/PPT

```json
{
    "channel": "OFFICE_OPEN_${TYPE}_EDITOR",
    "params": {
        "target": "http://127.0.0.1:8080/1.docs",
        "saveUrl": "http://127.0.0.1:8080/saveUrl",
        "filename": "xxxx.docs",
    }
}
```

| 名称 | 类型 | 描述
|----  |----  |------
|target | 字符串类型 | 要访问的文档地址，是一个url地址
|saveUrl | 字符串类型 | 要保存的服务器地址
|filename | 字符串类型 | 当前的文件名





