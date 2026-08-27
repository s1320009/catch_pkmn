# catch_pkmn
マウスドラッグの慣性移動で敵の猛攻を回避し、2軸チャージ投射でターゲットを捕獲する2Dアクションゲーム

## 📌 概要（Overview）
- **プレイヤースリング移動**: マウスの左ドラッグ＆リリースによる2点間ベクトル・慣性移動で、敵本体や放射状の螺旋弾幕を回避します。
- **2軸チャージ投射メカニクス**: WASDキーによる縦横独立パワーチャージと軌道予測線を駆使し、自機のボールをターゲットに狙い撃ちます。
- **自律型ステートマシンAI**: 敵ターゲットは状態遷移（思考・移動/テレポート・弾幕攻撃・ダッシュ）を備え、状況に応じた自律行動を行います。
- **コンポーネント指向 & エディタ機能（移行中）: 機能単位で拡張可能な GameObject 構造への刷新と、ステージ配置用インゲームエディタの構築を順次進行中です。

## 🚀 使い方（Usage）

### 1. ビルド・実行手順
1. リポジトリをクローンします。
   ```bash
   git clone [https://github.com/ユーザー名/catch_pkmn.git](https://github.com/ユーザー名/catch_pkmn.git)
   cd catch_pkmn
Visual Studio で catch_pkmn.sln（または .slnx）を開きます。
ソリューションを右クリックし、「NuGet パッケージの復元」 を実行して raylib パッケージを取得します。
ビルド構成を Debug / x64 に設定し、「ローカル Windows デバッガー」（または F5）で実行します。
### 2. 操作方法（Controls）
操作               入力　　　　　　　　　　　　　　説明
自機移動（スリング）マウス左ドラッグ ＆ ドロップ    引っ張ったベクトルの大きさ・方向に応じて自機が慣性移動
横チャージ         A（左）/ D（右）長押し ➔ 離す  X軸方向の投射パワーを決定
縦チャージ         W（上）/ S（下）長押し ➔ 離す  Y軸方向の投射パワーを決定
ボール投射         SPACE                         チャージした軌道予測線に沿ってボールを発射
チャージキャンセル  B                             チャージ状態を1段階巻き戻す
ポーズ             P                             ゲームの一時停止
ルール             セレクト画面でRuleを選択        ルール画面表示
エディタモード      タイトル画面で E               シーンエディタ / インスペクタ画面へ移行

## 📁 ファイル構成（Directory）

```text
catch_pkmn/
├── include/                        # ヘッダーファイル群（クラス定義・インターフェース）
│   ├── Component.h                 # コンポーネント基底クラス
│   ├── GameObject.h                # ゲームオブジェクト基底クラス・ライフサイクル管理
│   ├── TextureComponent.h          # 静止画テクスチャ描画コンポーネント
│   ├── TextureAnimeComponent.h     # アニメーション制御コンポーネント
│   ├── Texture.h                   # テクスチャアセット一括管理・ロード定義
│   ├── Player.h                    # 自機クラス（慣性移動・ダメージ・無敵時間処理）
│   ├── Ball.h                      # ボールクラス（2軸チャージ・予測線・発射ステートマシン）
│   ├── pkmn.h                      # ターゲット共通基底・AIステートマシン
│   ├── Mewtwo.h                    # ミュウツー固有処理（テレポート移動・極座標螺旋弾幕）
│   ├── Stage.h                     # ステージデータ定義・スポーン管理
│   ├── GameState.h                 # ゲーム全体のシーン状態（GameState）定義
│   ├── StateSelect.h               # ステージセレクト画面制御
│   ├── Rule.h                      # ルール説明画面制御
│   ├── ContinueSelect.h            # コンティニュー・リトライ選択UI
│   ├── BlinkingText.h              # UI点滅アニメーション制御
│   ├── Editor.h                    # インゲームエディタ（ヒエラルキー・シーン・インスペクタ）
│   └── Music.h                     # BGM・SEストリーミング再生制御
├── src/                            # ソースファイル群（実装ロジック）
│   ├── GameObject.cpp
│   ├── TextureComponent.cpp
│   ├── TextureAnimeComponent.cpp
│   ├── Texture.cpp
│   ├── Player.cpp
│   ├── Ball.cpp
│   ├── Pkmn.cpp
│   ├── Mewtwo.cpp
│   ├── Stage.cpp
│   ├── StateSelect.cpp
│   ├── Rule.cpp
│   ├── ContinueSelect.cpp
│   ├── BlinkingText.cpp
│   ├── Editor.cpp
│   ├── Music.cpp
│   └── main.cpp                    # エントリーポイント・メインループ・衝突判定
├── resources/                      # ゲームアセット（画像・音源・フォント）
│   ├── backColor.png               # 背景テクスチャ
│   ├── player_idle.png             # プレイヤーアニメーション素材
│   ├── gameBGM.mp3                 # ゲームプレイBGM
│   ├── loseBGM.mp3                 # ゲームオーバーBGM
│   ├── pauseSE.mp3                 # ポーズSE
│   └── myFont.ttf                  # 日本語フォントデータ
└── catch_pkmn.slnx                 # Visual Studio ソリューション構成ファイル

## 👤 作者（Author）
- **Name**: 菊池 誠人 (Makoto Kikuchi)
- **Affiliation**: 会津大学 コンピュータ理工学部
- **GitHub**: [https://github.com/s1320009]
### 🛠 Skills & Tools
- **Languages**: C++ (C++17), C
- **Frameworks & Libraries**: raylib
- **Architecture & Paradigms**: 
  - オブジェクト指向 / コンポーネント指向設計
  - 有限ステートマシン (FSM)
  - データ駆動設計 (Data-Driven)
- **Tools & Environments**: 
  - Visual Studio 2026
  - Git / GitHub
  - Aseprite（スプライトシート・2Dアニメーション制作）
