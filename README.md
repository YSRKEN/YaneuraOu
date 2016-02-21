# About this project

shogi engine(AI player), stronger than Bonanza6 , educational and tiny code(about 2500 lines) , USI compliant engine , capable of being compiled by VC++2015

将棋の思考エンジンで、Bonanza6より強く、教育的で短いコード(2500行程度)で書かれたUSIプロトコル準拠の思考エンジンで、VC++2015でコンパイル可能です。

[やねうら王mini 公式サイト (解説記事等)](http://yaneuraou.yaneu.com/YaneuraOu_Mini/)

[やねうら王公式 ](http://yaneuraou.yaneu.com/)

# Sub-projects

## やねうら王nano

やねうら王nanoは1500行程度で書かれた将棋AIの基本となるプログラムです。探索部は150行程度で、非常にシンプルなコードで、αβ以外の枝刈りを一切していません。(R2000程度)

## やねうら王nano plus(作業中2016年2月中旬ごろ完成予定)

やねうら王nano plusは、探索部250行程度で、オーダリングなどを改善した非常にシンプルでかつそこそこ強い思考エンジンです。(R2500程度を目指します)
	
## やねうら王mini (作業中2016年2月下旬完成予定)

やねうら王miniは、将棋の思考エンジンです。Bonanza6より強く、教育的かつ短いコードで書かれています。2500行程度、探索部400行程度。

## やねうら王classic 

やねうら王classicは、ソースコード4000行程度でAperyと同等の棋力を実現するプロジェクトです。(予定)

## やねうら王2016 

やねうら王 思考エンジン 2016年版(非公開)

## 連続自動対局フレームワーク

連続自動対局を自動化できます。 

## やねうら王協力詰めsolver
	
『寿限無3』(49909手)も解ける協力詰めsolver →　[解説ページ](http://yaneuraou.yaneu.com/2016/01/02/%E5%8D%94%E5%8A%9B%E8%A9%B0%E3%82%81solver%E3%82%92%E5%85%AC%E9%96%8B%E3%81%97%E3%81%BE%E3%81%99/)

## やねうら王評価関数バイナリ

CSAのライブラリの[ダウンロードページ](http://www.computer-shogi.org/library/)からダウンロードできます。


#　俺の作業メモ(2016/02/22 2:30現在)

- [ ] 長手数の詰将棋ルーチン作る
- [ ] やねうら王miniの探索部
- [ ] 2016/02/22・やねうら王nano plusの開発終了。

- [x] 2016/02/22・やねうら王nano plusに千日手判定追加。
- [x] 2016/02/22・Position::is_draw()実装
- [x] 2016/02/21・打ち歩詰め関係、ソース整理。
- [x] 2016/02/21・指し手生成で打ち歩詰め除外したときに、pseudo_legal()に打ち歩詰め判定入れるの忘れていたので修正。
- [x] 2016/02/21・Position::capture()の処理間違っていたの修正。
- [x] 2016/02/21・やねうら王nano plus、full depth searchの処理修正と調整。(+R100)
- [x] 2016/02/21・やねうら王nano plus、improvingフラグ用意。(+R5)
- [x] 2016/02/21・やねうら王nano plus、通常探索でevaluate()を呼び出すように変更。(+R50)
- [x] 2016/02/21・local game server、エンジン側から非合法手が送られてきたときにMOVE_RESIGN扱いにするように。
- [x] 2016/02/21・打ち歩詰めの判定、高速化。(+R10)
- [x] 2016/02/21・pos.legal()、玉の影の利きがあるときの処理間違っていたので修正。
- [x] 2016/02/21・local game serverでserver側の負荷が高かったのを修正。
- [x] 2016/02/21・local game serverで思考エンジンがbestmoveを返さないとハングしていたでタイムアウト処理追加。
- [x] 2016/02/21・やねうら王nano plusのMovePickerのRECAPTUREの処理、修正。
- [x] 2016/02/20・やねうら王nano plusの静止探索の指し手生成を色々調整。(+R180)。
- [x] 2016/02/20・やねうら王nano plusの静止探索で置換表絡みの処理追加。
- [x] 2016/02/19・やねうら王nano plusにLMRの導入。(+R140程度)
- [x] 2016/02/11・起動時にファイルからコマンドを受け付ける機能追加。
- [x] 2016/02/11・起動時にargvとして複数行を実行する機能追加。
- [x] 2016/02/10・自己対局サーバー、1スレッド×同時対局(マルチスレッド)に対応させる。
- [x] 2016/02/09・Threadクラス大改修。→ thread絡みの問題が起きなくなった。(ようだ)
- [x] 2016/02/09・_mm_mallocが失敗する件、さらに調査。→ std::thread絡みの問題くさい。
- [x] 2016/02/09・_mm_malloc()自作した。→これでも失敗する。newに失敗しているのか。ランタイム腐ってるのか…。
- [x] 2016/02/09・ローカルサーバー機能、ときどき対戦が開始しないので原因を調査する。→_mm_mallocが高負荷状態で失敗するようだ…。
- [x] 2016/02/09・やねうら王nano plusにkiller moveの導入。
- [x] 2016/02/09・nano plusで1手詰めなのに探索局面が多すぎるのでmate distance pruning追加した。
- [x] 2016/02/09・ベンチマークコマンド書けた。
- [x] 2016/02/08・やねうら王nano plusでMovePickerでの指し手生成を逐次生成に変更。
- [x] 2016/02/08・やねうら王nano plusで1手詰め判定を呼び出すように。
- [x] 2016/02/08・gcc/Clangでコンパイル通るようにする→Clangは無理。gccは行けそうだが、時間かかりそうなので保留。
- [x] 2016/02/08・やねうら王nano plusの開発開始。
- [x] 2016/02/08・やねうら王nano、floodgateでR2000を超えたので、nanoの開発終了。
- [x] 2016/02/08・実行ファイルをプロジェクトの一部として配布する。
- [x] 2016/02/07・やねうら王nanoをnonPV/PVの処理をきちんと書く。(これでR2000ぐらいになっていればnanoの開発は終了)
- [x] 2016/02/07・KPPの評価関数の差分実装完了。
- [x] 2016/02/06・やねうら王nanoで定跡の採択確率を出力するようにした。
- [x] 2016/02/06・やねうら王nanoの定跡を"book.db"から読み込むように変更。
- [x] 2016/02/06・定跡の読み込み等をこのフレームワークの中に入れる
- [x] 2016/02/06・定跡生成コマンド"makebook"
- [x] 2016/02/05・やねうら王nanoの探索部
- [x] 2016/02/05・やねうら王nano秒読みに対応。
- [x] 2016/02/04・やねうら王nanoに定跡追加。
- [x] 2016/02/01・新しい形式に変換した評価関数バイナリを用意する →　用意した → CSAのサイトでライブラリとして公開された。
- [x] 2016/01/31・やねうら王nanoの探索部