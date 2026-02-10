#include "DxLib.h"
#include "Gauge.h"


// ゲーム場面　名前
enum GameState
{
    STATE_TITLE,    // タイトル画面
    STATE_GAME,     // ゲーム本編
    STATE_RESULT    // 結果発表
};

// メイン関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //  1. 初期化エリアInit
    ChangeWindowMode(TRUE);             // ウィンドウモードにする
    if (DxLib_Init() == -1) return -1;  // DXライブラリ初期化
    SetDrawScreen(DX_SCREEN_BACK);      // 描画を裏画面にする（チラつき防止）

    // 今どの場面にいるか 変推
    GameState currentScene = STATE_TITLE;

    // キー入力の状態を受け取る箱
    char keyState[256];



    Gauge powerGauge;

    powerGauge.LoadAssets();

    //  2. メインループ
    while (ProcessMessage() == 0)
    {

        // 画面をクリア
        ClearDrawScreen();

        // キーボードの全キーの入力状態を取得
        GetHitKeyStateAll(keyState);

        // 3. 場面ごとの分岐（Switch / If-Else）
        switch (currentScene) {

            // ------------------------------------------------
            // ▼ タイトル画面の処理
        case STATE_TITLE:
            DrawString(200, 200, "【タイトル画面】", GetColor(255, 255, 255));
            DrawString(200, 240, "Zキーを押してスタート！", GetColor(255, 255, 0));

            // Zキーが押されたらゲーム画面へ移動
            if (keyState[KEY_INPUT_Z] == 1)
            {
                powerGauge.Init(2.0f, 100, 300, GetColor(255, 0, 0));
                currentScene = STATE_GAME;
                WaitTimer(200); // 押しっぱなし防止
            }
            break;


            // ▼ ゲーム本編の処理　　　　ここにゲージ処理
        case STATE_GAME:

            powerGauge.Update();
            powerGauge.Draw();

            DrawString(200, 200, "【ゲーム中】", GetColor(255, 255, 255));
            DrawString(200, 240, "スペースキーで止める", GetColor(0, 255, 255));

            // スペースキーが押されたら結果画面へ
            if (keyState[KEY_INPUT_SPACE] == 1)
            {
                powerGauge.Stop();
                currentScene = STATE_RESULT;
                WaitTimer(200);
            }
            break;



            ///////////////////////////////////////////////////////////////////////////////////////
                // ▼ 結果画面の処理
        case STATE_RESULT:
            DrawString(200, 200, "【結果発表】", GetColor(255, 255, 255));
            DrawString(200, 240, "Zキーでタイトルに戻る", GetColor(0, 255, 0));

            // Zキーが押されたらタイトルへ戻る      ループ
            if (keyState[KEY_INPUT_Z] == 1) {

                currentScene = STATE_TITLE;
                WaitTimer(200);
            }
            break;
        }

        //  4. 待機＆画面反映（Wait / Flip）
        ScreenFlip();
    }

    DxLib_End(); // 終了処理
    return 0;
}