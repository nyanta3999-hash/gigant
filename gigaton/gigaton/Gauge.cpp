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
void Gauge::Init(float startSpeed, int x, int y, int c) {
    value = 0.0f;
    speed = startSpeed;
    isStopped = false;
    posX = x;
    posY = y;
    color = c;
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
    // 　　　　　　　　赤いバー
    // -----------------------------------------------------
    DrawBox(posX, posY, posX + 200, posY + 30, GetColor(50, 50, 50), TRUE);
    int w = (int)(value * 2.0f);
    DrawBox(posX, posY, posX + w, posY + 30, color, TRUE);


    // -----------------------------------------------------
    // 　　　　　 バーの上にスライムを描く
    // -----------------------------------------------------

    // 表示する場所（バーの真ん中よりちょっと上）
    // 画像サイズ 64
    int charX = posX + (200 / 2) - 32;
    int charY = posY - 80;

    // 　もし止まっていたら
    if (isStopped == true)
    {
        // 岩を表示
        DrawGraph(charX, charY, imgStone, TRUE);
    }
    // まだ動いていたら　溜め中
    else
    {
        // ここで　　プルプル震える　　演出を入れる
        // value（パワー）が大きいほど、震え幅（shake）を大きくする
        int shake = (int)(value / 5); // こんかいは５でいったん

        // 乱数でちょっとだけ座標をズラす
        int randX = GetRand(shake) - (shake / 2);
        int randY = GetRand(shake) - (shake / 2);

        // 「ためのスライム」を表示
        DrawGraph(charX + randX, charY + randY, imgCharge, TRUE);
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