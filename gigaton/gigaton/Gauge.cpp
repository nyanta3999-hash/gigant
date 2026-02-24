#include "DxLib.h"
#include "Gauge.h"
#include <math.h>

//画像
void Gauge::LoadAssets() {
    imgIdle = LoadGraph("image/slime_idle.png");
    imgJump = LoadGraph("image/slime_jump.png");
    imgCharge = LoadGraph("image/slime_charge.png");
    imgStone = LoadGraph("image/slime_stone.png");
    imgFalling = LoadGraph("image/slime_falling.png");
    imgImpact = LoadGraph("image/slime_impact.png");
}


void Gauge::Init(float startSpeed, int x, int y, int c, int mode) {
    value = 0.0f;
    speed = startSpeed;
    isStopped = false;
    posX = x;
    posY = y;
    color = c;
    gameMode = mode;//モードの記録
    //ここ修正しまう　入場とジャンプの初期の回数は1回目だけです
    //スタート時のみ
    if (mode == 1) 
    {
        moveX = -100.0f;
        JumpY = 0.0f;//最初は地面
        JumpSpeed = -12.0f;// 上に向かう勢い
        //受け取ったモードを保存
    }
 
}

bool Gauge::UpdateEntrance() {
    moveX += 5.0f;

    if (moveX >= posX) {
        moveX = posX;
        return true;
    }

    return false;
}

bool Gauge::UpdateJump() {
    //高さの変化
    JumpY += JumpSpeed;

    //重力
    JumpSpeed += 0.8f;

    //もし勢いがなくなったら
    if (JumpSpeed >= 0.0f) {
        JumpSpeed = 0.0f;
        return true;
    }
    return false;

}

bool Gauge::UpdateFall() {
    //落ちるスピード
    //jumpspeedはplusで下方向に落ちる
    //固定スピードで落とす
    JumpSpeed = 5.0f;

    //座標を更新
    JumpY += JumpSpeed;

    //座標を更新
    JumpY += JumpSpeed;

    //地面に着いたか判定
    //JumpYが０以上になったら地面判定
    if (JumpY >= 0.0f) {
        JumpY = 0.0;    //めり込まないように０に
        JumpSpeed = 0.0f;//勢いをとめる
        return true;//着地の報告をする
    }
    return false; //落下中です
}


void Gauge::Update()
{
    if (isStopped) return;

    value += speed;

    // 0〜100の間を往復させる
    if (value >= 100.0f || value <= 0.0f)
    {
        speed *= -1;
    }
}

// 
void Gauge::Draw()
{
    // -----------------------------------------------------
    // 1. 変数の準備
    // -----------------------------------------------------
    int currentDrawX;

    // 基本の高さ（地面）を入れておく
    int currentDrawY = (posY + 200) - 80;


    // -----------------------------------------------------
    // 2. 座標の計算
    // -----------------------------------------------------

    // ■ X座標（入場中かどうか）
    if (moveX < posX) 
    {
        currentDrawX = (int)moveX;
        // 入場中は「地面」のままでいいので、currentDrawY は何もしない
    }
    else 
    {
        currentDrawX = posX;

        // ■ Y座標（ジャンプの計算）
        // ここで「JumpY（高さ）」を足し込む！
        currentDrawY = ((posY + 200) - 80) + (int)JumpY;
    }


    // -----------------------------------------------------
    // 3. 描画
    // -----------------------------------------------------



    // キャラクター
    int charX = currentDrawX + 50;
    int charY = currentDrawY;

    //ゲージをgamemodeで一括管理させるために最初のゲージも移動させました
    //↓

    if (gameMode == 1) 
    {
        // 枠
        DrawBox(posX, posY, posX + 30, posY + 200, GetColor(50, 50, 50), TRUE);

        // 中身
        int h = (int)(value * 2.0f);
        DrawBox(currentDrawX, posY + 200 - h, currentDrawX + 30, posY + 200, color, TRUE);
    }

    //モード２　第二のゲージ処理　振り子型
    else if (gameMode == 2)
    {             // 振り子の全体の横幅　200ピクセル
        int width = 200;
        int baseY = posY + 100;// スライムの横当たりに表示するための高さ
        
                //  1  背景のレールを書く　暗いグレーで
        DrawBox(posX, baseY, posX + width, baseY + 20, GetColor(50, 50, 50), TRUE);
                // 2 真ん中を描く　ど真ん中前後　10ピクセル
        int centerX = posX + (width / 2);
        DrawBox(centerX - 10, baseY, centerX + 10, baseY + 20, GetColor(255, 255, 0), TRUE);

        //動くカーソルのX座標を計算
        //valueは0から１００なので　2倍すれば0から200ピクセルの間を動くようになる
        int cursorX = posX + (int)(value * 2.0f);

        //４　カーソルの描画　(太目の縦線　＋　〇)
        DrawLine(cursorX, baseY - 10, cursorX, baseY + 30, color, 5);
        DrawCircle(cursorX, baseY + 10, 8, color, TRUE);
    }

    //モード３ 第三のゲージ処理　広がる円
    else if (gameMode == 3)
    {   //円の中心を決める
        int centerX = posX + 30;
        int baseY = posY + -50;
        //1 目標となる枠を書く
        //半径　80　の固定サイズ　FALSEにすると枠線だけになる
        DrawCircle(centerX, baseY, 80, GetColor(255, 255, 255), FALSE, 5);

        //　2　動く円の半径を計算する
        //valueは０から１００だから少し大きめに広がるように1，2倍にして
        //これで最大120まで広がる　枠の80を通り越して失敗する動きができる
        int currentRadius = (int)(value * 1.2f);

        // 3　動く円の中身を書く
        //TRUEにして塗りつぶし
        DrawCircle(centerX, baseY, currentRadius, color, FALSE, 5);

    }

    //ここからキャラの描写/    共通です       ////////////////////////////////////////////////////////////////////////////

    // 画像の出し分け
    if (moveX < posX) // 入場中
    {
        DrawGraph(charX, charY, imgIdle, TRUE);
    }
    else if (JumpY < 0 && isStopped == false && value == 0) // ジャンプ中
    {
        DrawGraph(charX, charY, imgJump, TRUE);
    }//落下中
    //パワー決定済み　isStoppedかつまだ空中JumpY < 0 なら落下画像に切り替え
    else if(isStopped == true && JumpY <0)
    {
        DrawGraph(charX, charY, imgFalling, TRUE);
    }//結果　地面について止まっている
    else if (isStopped == true) // 結果
    {
        DrawGraph(charX, charY, imgStone, TRUE);
    }
    else // 溜め
    {
        int shake = (int)(value / 5);
        DrawGraph(charX + GetRand(shake), charY + GetRand(shake), imgCharge, TRUE);
    }
}

void Gauge::Stop()
{
    isStopped = true;
}

bool Gauge::IsStopped()
{
    return isStopped;
}

float Gauge::GetValue()
{
    return value;
}

void Gauge::DrawIdle(){
    //画面の真ん中当たりに表示する感じで
    DrawGraph(280, 200, imgIdle, TRUE);

}

void Gauge::DrawImpact() {
    //迫力を出すために少しずらしたい
    DrawGraph(posX + 50, (posY + 200) - 80,imgImpact, TRUE);
}

//点数を計算する処理

float Gauge::GetScore() {
    float score = 0.0f;

    // モード1：パワー（赤いバー）
    if (gameMode == 1) {
        score = value;
    }

    // モード2：振り子（青いカーソル）
    // 真ん中（50）がパーフェクト
    else if (gameMode == 2) {
        // ★修正：ここは currentRadius じゃなくて「value - 50.0f」です！
        float diff = (float)fabs(value - 50.0f);
        score = 100.0f - (diff * 2.0f);
        if (score < 0.0f) {
            score = 0.0f;
        }
    }

    // モード3：円（緑の輪っか）
    // ★追加：ここがまるごと抜けていました！
    else if (gameMode == 3) {
        // ここで初めて currentRadius くんが誕生します！
        float currentRadius = value * 1.2f;

        // 枠（80）からどれくらいズレているか計算
        float diff = (float)fabs(currentRadius - 80.0f);

        score = 100.0f - (diff * 1.25f);
        if (score < 0.0f) {
            score = 0.0f;
        }
    }

    return score;
}
