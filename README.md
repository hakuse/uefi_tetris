# uefi_tetris

UEFIの勉強としてTetrisを作ってみました。SDKは使っていません。

## 注意
このプログラムを使用、実行したことによる機器の故障等の損害については全て自己責任でお願いします。（私は一切責任を負いません）

## 参考にさせていただいたサイト
- https://shina-ecc.hatenadiary.org/entry/20140819/1408434995
- http://yuma.ohgami.jp/UEFI-Bare-Metal-Programming-2/04_evt_timer.html
- https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/5_uefi_services/51_services_that_uefi_drivers_commonly_use/515_event_services
- http://yuma.ohgami.jp/UEFI-Bare-Metal-Programming-2/02_simple_text_input_ex.html
- https://shina-ecc.hatenadiary.org/entry/20140819/1408434995
- https://wiki.osdev.org/UEFI_App_Bare_Bones#Creating_and_running_the_CD_image
- https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/3_foundation/35_guids

## 開発環境
WSL2 Ubuntu 20.04.5 LTS
    
## VirtualBox上での実行方法
### コンパイル
開発環境上で以下を実行  
```
$make
```
    
### 準備
VirtuslBox上で仮想マシンを新規作成
  - タイプ:Other
  - バージョン:Other/Unknown (64-bit)
  - 「EFIを有効化」にチェックを入れる
  - ストレージのストレージデバイスにコントローラー:USBを追加し、USBの光学メディアとしてコンパイルで作成されたtetris.isoを指定
  
### 実行
仮想マシンを起動
  - 1回目は失敗するときがある。（原因不明）
  - EFI shellが表示された場合は以下のコマンドを順に実行する。
```
>fs0:
>cd /efi/boot/
>bootx64.efi
```
        
### 終了
仮想マシンの閉じるボタンで閉じてください。
   
## 実機での実行方法（非推奨）
### コンパイル
開発環境上で以下を実行
```
$make tetris.efi
```
     
### 準備
- USBメモリを用意し(このUSBメモリ内のデータは消えるため注意)、FAT32形式でフォーマットする。
- USBメモリ内にefiというディレクトリを作成する
- efiディレクトリ内にbootというディレクトリを作成する。
- bootディレクトリ内にコンパイルで作成したtetris.efiを配置する。
     
### 実行
- PCの起動前にUSBメモリを挿しておき、USBメモリから起動するようにBootの順番を変更する。
- Secure BootがONになっているPCではOFFにする。
     
### 終了
シャットダウン方法をプログラム内に組み込んでいないため、電源ボタンで電源を落としてください。
    
## 操作方法
- 上矢印キー 右回転
- 右矢印キー 右へ移動
- 左矢印キー 左へ移動
- 下矢印キー ソフトドロップ
- スペースキー ハードドロップ
    
## 実装していないもの ~~（今後実装するとは言っていない）~~
- ホールド機能
- ネクストを表示する機能
- リセット機能
- シャットダウン機能
    
## バグ
- 特定の操作を行った際に急にプログラムが落ちるバグ（原因不明）
    
