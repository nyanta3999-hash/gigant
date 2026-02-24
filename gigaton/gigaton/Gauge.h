#pragma once

// クラスの定義
class Gauge {
private:
    // ■ 変数（データを入れる箱）
    float value;      // ゲージの数値（0.0〜100.0）
    float speed;      // 増えるスピード
    bool  isStopped;  // 止まったかどうかのフラグ
    int   posX, posY; // 表示する場所（X, Y）
    int   color;      // 色
    //３回システムの一括管理クラス
    int gameMode;

    //入場アニメーション用の変数
    float moveX;
    float JumpY;//今の高さ 0なら地面,マイナスなら空中 
    float JumpSpeed;//ジャンプの勢い
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
    void Init(float startSpeed, int x, int y, int c, int mode);

    //teue が帰ってきたら到着、falseならまだ移動中とする
    bool UpdateEntrance(); //入場用のアニメーション関数
    bool UpdateJump();
    bool UpdateFall();
    void Update();
    void Draw();
    void Stop();
    void DrawIdle();//待機のスライムを書く
    void DrawImpact();//ドスンと衝撃スライムを書く


    bool IsStopped(); // 止まっているか確認する機能
    float GetValue(); // 今の数値を取り出す機能
    float GetScore();
};