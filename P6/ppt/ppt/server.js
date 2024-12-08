const express = require('express');
const app = express();
const port = 3000;

// 设定静态文件目录
app.use(express.static('export'));

app.listen(port,'0.0.0.0', () => {
  console.log(`服务器在 http://localhost:${port} 启动`);
});
