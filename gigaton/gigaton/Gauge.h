#pragma once

// クラスの定義（設計図）
class Gauge {
private:
    // ■ 変数（データを入れる箱）
    float value;      // ゲージの数値（0.0〜100.0）
    float speed;      // 増えるスピード
    bool  isStopped;  // 止まったかどうかのフラグ
    int   posX, posY; // 表示する場所（X, Y）
    int   color;      // 色

  // Gauge.h の private の部分に追加
    int imgIdle;      // 待機
    int imgJump;      // ジャンプ
    int imgCharge;    // 溜め
    int imgStone;     // 岩
    int imgFalling;   // 落下
    int imgImpact;    // 衝撃

public:
    // 関数
    void LoadAssets(); // 画像の読み込み
    void Init(float startSpeed, int x, int y, int c);
    void Update();
    void Draw();
    void Stop();


    bool IsStopped(); // 止まっているか確認する機能
    float GetValue(); // 今の数値を取り出す機能
};