<?php if (!defined('THINK_PATH')) exit(); /*a:1:{s:79:"/Library/WebServer/project/public/../application/index/view/index/showinfo.html";i:1555263605;}*/ ?>
<!doctype html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <title>Get key and iv</title>
        <style>
        body{
            color: #333;
            font: 16px Verdana, "Helvetica Neue", helvetica, Arial, 'Microsoft YaHei', sans-serif;
            margin: 0px;
            padding: 20px;
        }
        a{
            color: #868686;
            cursor: pointer;
        }
        a:hover{
            text-decoration: underline;
        }
        h2{
            color: #4288ce;
            font-weight: 400;
            padding: 6px 0;
            margin: 6px 0 0;
            font-size: 28px;
            border-bottom: 1px solid #eee
        }
        div{
            margin: 8px;
        }
        .info{
            padding: 12px 0;
            border-bottom: 1px solid #eee;
        }
        .copyright{
            margin-top: 24px;
            padding: 12px 0;
            border-top: 1px solid #eee;
        }
        </style>
    </head>
    <center>
    <body>
        <h2>用户信息</h2>
        <div class="info">
            感染序列 : <?php echo $vict_info['id']; ?><br/>
            机器名称 : <?php echo $vict_info['pcname']; ?><br/>
            AES密钥 : <?php echo $vict_info['AESkey']; ?><br/>
            AES--IV : <?php echo $vict_info['AESiv']; ?><br/>
        </div>
        <div class="copyright">
            <a title="Know More" href="https://github.com/YZ-WANG/Hidden-tear">Know more about</a>
            <span>Hidden-tear</span>
            <span>{Xrosheart}</span>
        </div>
    </body>
    </center>
</html>