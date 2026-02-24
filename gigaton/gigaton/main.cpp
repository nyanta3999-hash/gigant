#include "DxLib.h"
#include "Gauge.h"


// ゲーム場面　名前
enum GameState
{
    STATE_TITLE,    // タイトル画面
    STATE_ENTRY,
    STATE_JUMP,
    STATE_GAME,     // ゲーム本編
    STATE_FALL,     //落下
    STATE_IMPACT,   //床をたたく
    STATE_RESULT    // 結果発表

};

// メイン関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //  1. 初期化エリアInit
    ChangeWindowMode(TRUE);             // ウィンドウモードにする
    if (DxLib_Init() == -1) return -1;  // DXライブラリ初期化

    // ① フォントのファイルを読み込む
    AddFontFile("image/misaki_gothic.ttf");

    // ② 【追加】ゲーム全体の普通の文字（DrawStringなど）を全部このフォントにする
    ChangeFont("美咲ゴシック");

    // ③ デカい文字専用の筆を作る（名前を "美咲ゴシック" に修正）
    int fontScore = CreateFontToHandle("美咲ゴシック", 64, 5, DX_FONTTYPE_ANTIALIASING_EDGE);

    SetDrawScreen(DX_SCREEN_BACK);      // 描画を裏画面にする（チラつき防止）


    // 今どの場面にいるか 変推/////////////////////////////////////////////////////////////////////////////////////////////
    GameState currentScene = STATE_TITLE;//タイトルからスタート
    //ラウンド数と合計スコアを覚える変数
    int currentRound = 1;           //今何回目か　１から３
    float totalScore = 0.0f;        //3回文の合計スコア     
    float npcScore = 0.0f;          //ライバルのスコア
    float npcX = 500.0f;            //ライバルの立ち位置


    // キー入力の状態を受け取る変数
    char keyState[256];
    //新たな変数　フレームの数を数える
    int stateTimer = 0;
 
    Gauge powerGauge;// ゲージ

    powerGauge.LoadAssets();// 画像を読み込む

    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int imgEarth[5];       //地球の画像4枚
    int imgBg;      //背景
    int finalEarthLevel = 0;    //今回のスコアでどのくらい割れる？
    int currentEarthLevel = 0;  //今の画面に表示しているひび割れの段階　パラパラ漫画の用に
    int resultTimer = 0;        //アニメーションの時間を図るタイマー

    //　地球画像読み込み
    imgEarth[0] = LoadGraph("image/earth.png"); //無傷
    imgEarth[1] = LoadGraph("image/earth2.png"); //ヒビ小
    imgEarth[2] = LoadGraph("image/earth3.png"); //ヒビ中
    imgEarth[3] = LoadGraph("image/earth4.png"); //大
    imgEarth[4] = LoadGraph("image/earth5.png"); //大

    imgBg = LoadGraph("image/bg.png");  //背景の画像　変数

    //  2. メインループ
    while (ProcessMessage() == 0)
    {
        // ループの 一番最初 で今の時間を記録
        int startTime = GetNowCount();
        // 画面をクリア
        ClearDrawScreen();
        //　リザルト画面以外の時は1番奥に背景を書く
        if (currentScene != STATE_RESULT) {
            DrawGraph(0, 0, imgBg, TRUE);
        }

        // キーボードの全キーの入力状態を取得
        GetHitKeyStateAll(keyState);

        // 3. 場面ごとの分岐（Switch / If-Else）
        switch (currentScene)
        {

            // -----------------------------------
            // ▼ タイトル画面の処理
        case STATE_TITLE:

       
            SetFontSize(32);

            DrawString(100, 50, "【タイトル画面】", GetColor(255, 255, 255));
            DrawString(100, 80, "Zキーを押してスタート！", GetColor(255, 255, 0));

            powerGauge.DrawIdle();

            // Zキーが押されたらゲーム画面へ移動
            if (keyState[KEY_INPUT_Z] == 1)
            {   
                currentRound = 1;
                totalScore = 0.0f;
                //1回目の初期化　最後の数字をCRRENTROUNDにする
                powerGauge.Init(2.0f, 100, 200, GetColor(255, 0, 0), currentRound);

                currentScene = STATE_ENTRY;
                WaitTimer(200); // 押しっぱなし防止
            }
            break;

        case STATE_ENTRY:

            SetFontSize(100);

            //背景や文字を書く
            DrawString(200, 100, "Ready", GetColor(255, 255, 0));

            SetFontSize(16);// 戻し

            //スライムを歩かせる
            //updateEntranceがtrueを返すとifの中に入る
            if (powerGauge.UpdateEntrance() ==true)
            {
                currentScene = STATE_JUMP;
                
            }

            //描画
            //移動中の座標で書く
            powerGauge.Draw();
            break;


        case STATE_JUMP:
            //ジャンプ処理　頂点に来たらtrueが変える
            if (powerGauge.UpdateJump() == true) 
            {
                currentScene = STATE_GAME;
            }
            powerGauge.Draw();
            break;
        

            // ▼ ゲーム本編の処理　　　　ここにゲージ処理
        case STATE_GAME:

            

            powerGauge.Update();
            powerGauge.Draw();

            SetFontSize(32);

            DrawString(200, 50, "【ゲーム中】", GetColor(255, 255, 255));
            DrawString(200, 120, "スペースキーで止める", GetColor(0, 255, 255));

            // スペースキーが押されたら結果画面へ
            if (keyState[KEY_INPUT_SPACE] == 1)
            {
                powerGauge.Stop();

                //今のモードの点数をもらって、合計点に足す
                float currentScore = powerGauge.GetScore();
                totalScore += currentScore;

                //あとでここにトータルスコアに点数を足す処理を書く
                if (currentRound < 3)
                {
                    currentRound++;

                    if (currentRound == 2) 
                    {
                        powerGauge.Init(3.0f, 100, 200, GetColor(0, 0, 255), currentRound);
                    }

                    else if (currentRound == 3) 
                    {
                        //3回目　緑の円　さらに早く
                        powerGauge.Init(4.0f, 100, 200, GetColor(0, 255, 0), currentRound);
                    }
                    WaitTimer(500);
                }
                else
                {
                    //追加 2月15日　3回目が終わる　合計スコア300までで割れ具合を判定します
                    if (totalScore < 160.0f)
                    {
                        finalEarthLevel = 0;//1
                    }
                    else if (totalScore < 220.0f)
                    {
                        finalEarthLevel = 1;//2
                    }
                    else if (totalScore < 250.f)
                    {
                        finalEarthLevel = 2;//3
                    }
                    else if (totalScore < 260.0f)
                    {
                        finalEarthLevel = 3; //4
                    }
                    else if (totalScore < 280.0f)
                    {
                        finalEarthLevel = 3; // 280点未満ならレベル3
                    }
                    else
                    {
                        finalEarthLevel = 4; // 280点「以上」なら問答無用で崩壊（レベル4）！
                    }

                    currentEarthLevel = 0;
                    resultTimer = 0;

                    currentScene = STATE_FALL;
                    WaitTimer(200);
             
                }

            }
            break;


        case STATE_FALL:
            //落下処理　地面に着いたらtrueが帰ってくるようにする
            if (powerGauge.UpdateFall() == true)
            {
                //地面に着いたらIMPACTへ
                //今のところRESULT
                currentScene = STATE_IMPACT;
                //タイマーを０にリセット
                stateTimer = 0;
            }
            powerGauge.Draw();//ここはまだ落下中
            break;

        case STATE_IMPACT:
            //z面に着いたら
            stateTimer++;//フレームごとに１追加
            if (stateTimer > 60) {
                currentScene = STATE_RESULT;
            }

            //描画
            //スライムの絵ではなく星割れやインパクト専用の描画を出したい
            //将来ここに星が割れる演出や画面シェイクを入れるz
            powerGauge.DrawImpact();
            break;



            ///////////////////////////////////////////////////////////////////////////////////////
                // ▼ 結果画面の処理
        case STATE_RESULT:
            //背景黒
            DrawBox(0, 0, 800, 600, GetColor(0, 0, 0), TRUE);

            //タイマーを進める
            resultTimer++;

            //20フレームごとにヒビを1段階進める
            //ただしfinalEarthLevelになったらとめる
            if (resultTimer % 20 == 0 && currentEarthLevel < finalEarthLevel) 
            {
                currentEarthLevel++;
            }

            DrawGraph(-20, -20, imgEarth[currentEarthLevel], TRUE); //画像のXY

            DrawFormatStringToHandle(100, 100, GetColor(255, 255, 0), fontScore, "SCORE: %.1f", totalScore);

            DrawFormatString(10, 50, GetColor(255, 0, 0), "IMAGE CHECK: %d", imgEarth[0]);

            if (keyState[KEY_INPUT_Z] == 1)
            {
                currentScene = STATE_TITLE;
                WaitTimer(200);
            }
            break;

            
        }


        //  4. 待機＆画面反映（Wait / Flip）
        ScreenFlip();

        int tookTime = GetNowCount() - startTime;
        int waitTime = 16 - tookTime;
        if (waitTime > 0) 
        {
            WaitTimer(waitTime);                  // もしPCが早すぎて時間が余ってたら、その分だけ待つ！
        }
    }

    DxLib_End(); // 終了処理
    return 0;
}