# ChatGPTによるBlock_fall_downのWindow版のデモ版を作成した

# Win32 Block Fall Down (Tetris Clone)

「Block Fall Down」は、Windows API（Win32）を使用してC++で構築されたシンプルなGUIテトリスゲームです。SFMLや外部ライブラリに依存せず、完全にWin32で構成されており、学習・教育用途にも適しています。

## 🧱 ゲームの特徴

- ブロック種類：
  - T型（赤）
  - L型（青）
  - O型（オレンジ）
  - I型（シアン）
  - J型（紫：逆L）
- 操作キー：
  - `A`: 左に移動
  - `D`: 右に移動
  - `W`: 回転
- 機能：
  - スコア計算（1行クリア = 100点）
  - 次のブロック表示（プレビュー付き）
  - グリッド表示
  - 60秒の制限時間
  - 残り時間の表示（画面右下）

## 🖼️ スクリーンショット

![image](https://github.com/user-attachments/assets/8750a11a-f485-447d-9e17-65518ef215d2)


## 💻 ビルド方法

1. **開発環境**
   - Windows OS
   - C++17 対応のコンパイラ（例：MinGW、MSVC）
   - CMake（CLionやVisual Studioでも可）

2. **ビルド手順**


3. **実行ファイル**



## 🧠 学習ポイント

- Win32 API の基本構文とイベントループ
- GUI 描画（`DrawEdge`, `FillRect`, `TextOutA`）
- タイマーイベントの処理（`SetTimer` / `KillTimer`）
- シンプルなブロック落下ロジック（グリッド上）

## 🎮 TODO（今後の改良案）

- ハイスコアの保存
- ブロックの落下速度の段階的加速
- マウス対応 or 設定画面
- 効果音・BGMの追加（WinMM など）

## 📝 ライセンス

MIT License © 2025 Seiya Genda

# Demo of Block Fall Down (Window version) created by ChatGPT

# Win32 Block Fall Down (Tetris Clone)

**Block Fall Down** is a simple GUI-based Tetris-like game built entirely in C++ using the Windows API (Win32).  
It does **not rely on SFML or any external libraries**, making it ideal for learning and educational purposes.

## 🧱 Game Features

- **Block Types**:
  - T-shape (Red)
  - L-shape (Blue)
  - O-shape (Orange)
  - I-shape (Cyan)
  - J-shape (Purple, reversed L)
- **Controls**:
  - `A`: Move Left
  - `D`: Move Right
  - `W`: Rotate
- **Gameplay Features**:
  - Score calculation (100 points per cleared line)
  - "Next block" preview
  - Grid rendering
  - 60-second time limit
  - Remaining time display (bottom right)

## 🖼️ Screenshot

![image](https://github.com/user-attachments/assets/92b5b2c0-7495-4d38-88f4-d0090c5d759d)


## 💻 How to Build

1. **Development Environment**
   - Windows OS
   - C++17 compatible compiler (e.g. MinGW, MSVC)
   - CMake (CLion or Visual Studio also supported)

2. **Build Instructions**  
   *(to be added depending on your build system)*

3. **Executable**  
   *(to be added, e.g. `./build/BlockFallDown.exe`)*

## 🧠 Learning Highlights

- Basic Win32 API structure and message loop
- GUI drawing with `DrawEdge`, `FillRect`, `TextOutA`
- Timer events using `SetTimer` / `KillTimer`
- Simple grid-based falling block mechanics

## 🎮 TODO (Future Improvements)

- High score saving
- Progressive speed-up of block drops
- Mouse support or settings screen
- Sound effects / BGM (e.g. using WinMM)

## 📝 License

MIT License © 2025 Seiya Genda

