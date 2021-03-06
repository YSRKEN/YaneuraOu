﻿#include "../shogi.h"

#if defined (USE_SEE)

#include "../position.h"

using namespace Eval;
using namespace Effect8;

namespace {

  // min_attacker()はsee()で使われるヘルパー関数であり、(目的升toに利く)
  // 手番側の最も価値の低い攻撃駒の場所を特定し、その見つけた駒をビットボードから取り除き
  // その背後にあった遠方駒をスキャンする。(あればstmAttackersに追加する)

  // またこの関数はmin_attacker<PAWN>()として最初呼び出され、PAWNの攻撃駒がなければ次に
  // KNIGHTの..というように徐々に攻撃駒をアップグレードしていく。

  // occupied = 駒のある場所のbitboard。今回発見された駒は取り除かれる。
  // stmAttackers = 手番側の攻撃駒
  // attackers = toに利く駒(先後両方)。min_attacker(toに利く最小の攻撃駒)を見つけたら、その駒を除去して
  //  その影にいたtoに利く攻撃駒をattackersに追加する。
  // stm = 攻撃駒を探すほうの手番
  // uncapValue = 最後にこの駒が取れなかったときにこの駒が「成り」の指し手だった場合、その価値分の損失が
  // 出るのでそれが返る。

  // 返し値は今回発見されたtoに利く最小の攻撃駒。これがtoの地点において成れるなら成ったあとの駒を返すべき。

  template <Color stm>
  Piece min_attacker(const Position& pos, const Square& to
	  , const Bitboard& stmAttackers, Bitboard& occupied, Bitboard& attackers
  ) {

	  // 駒種ごとのbitboardのうち、攻撃駒の候補を調べる
  //:      Bitboard b = stmAttackers & bb[Pt];

	  // 歩、香、桂、銀、金、角、飛、馬、龍…の順で取るのに使う駒を調べる。

	  Bitboard b;
	  b = stmAttackers &  pos.piece_bb[PIECE_TYPE_BITBOARD_PAWN][stm];   if (b) goto found;
	  b = stmAttackers &  pos.piece_bb[PIECE_TYPE_BITBOARD_LANCE][stm];  if (b) goto found;
	  b = stmAttackers &  pos.piece_bb[PIECE_TYPE_BITBOARD_KNIGHT][stm]; if (b) goto found;
	  b = stmAttackers &  pos.piece_bb[PIECE_TYPE_BITBOARD_SILVER][stm]; if (b) goto found;
	  b = stmAttackers &  pos.piece_bb[PIECE_TYPE_BITBOARD_GOLD][stm];   if (b) goto found;
	  b = stmAttackers & ~pos.piece_bb[PIECE_TYPE_BITBOARD_HDK][stm] & pos.piece_bb[PIECE_TYPE_BITBOARD_BISHOP][stm]; if (b) goto found;
	  b = stmAttackers & ~pos.piece_bb[PIECE_TYPE_BITBOARD_HDK][stm] & pos.piece_bb[PIECE_TYPE_BITBOARD_ROOK][stm];   if (b) goto found;
	  b = stmAttackers &  pos.piece_bb[PIECE_TYPE_BITBOARD_HDK][stm] & pos.piece_bb[PIECE_TYPE_BITBOARD_BISHOP][stm]; if (b) goto found;
	  b = stmAttackers &  pos.piece_bb[PIECE_TYPE_BITBOARD_HDK][stm] & pos.piece_bb[PIECE_TYPE_BITBOARD_ROOK][stm];   if (b) goto found;

	  // 攻撃駒があるというのが前提条件だから、以上の駒で取れなければ、最後は玉でtoの升に移動出来て
	  // 駒を取れるはず。

	  // ここでサイクルは停止するのだ。
	  return KING;

  found:;

	  // bにあった駒を取り除く

	  Square sq = b.pop();
	  occupied ^= sq;

	  // このときpinされているかの判定を入れられるなら入れたほうが良いのだが…。
	  // この攻撃駒の種類によって場合分け

	  const auto& bb = pos.piece_bb;

	  auto dirs = directions_of(to, sq);
	  if (dirs) switch (pop_directions(dirs))
	  {
	  case DIRECT_RU: case DIRECT_RD: case DIRECT_LU: case DIRECT_LD:
		  // 斜め方向なら斜め方向の升をスキャンしてその上にある角・馬を足す
		  attackers |= bishopEffect(to, occupied) & (bb[PIECE_TYPE_BITBOARD_BISHOP][BLACK] | bb[PIECE_TYPE_BITBOARD_BISHOP][WHITE]);

		  ASSERT_LV3((bishopStepEffect(to) & sq));
		  break;

	  case DIRECT_U:
		  // 後手の香 + 先後の飛車
		  attackers |= rookEffect(to, occupied) & lanceStepEffect(BLACK, to)
			  & (bb[PIECE_TYPE_BITBOARD_ROOK][BLACK] | bb[PIECE_TYPE_BITBOARD_ROOK][WHITE] | bb[PIECE_TYPE_BITBOARD_LANCE][WHITE]);

		  ASSERT_LV3((lanceStepEffect(BLACK, to) & sq));
		  break;

	  case DIRECT_D:
		  // 先手の香 + 先後の飛車
		  attackers |= rookEffect(to, occupied) & lanceStepEffect(WHITE, to)
			  & (bb[PIECE_TYPE_BITBOARD_ROOK][BLACK] | bb[PIECE_TYPE_BITBOARD_ROOK][WHITE] | bb[PIECE_TYPE_BITBOARD_LANCE][BLACK]);

		  ASSERT_LV3((lanceStepEffect(WHITE, to) & sq));
		  break;

	  case DIRECT_L: case DIRECT_R:
		  // 左右なので先後の飛車
		  attackers |= rookEffect(to, occupied)
			  & (bb[PIECE_TYPE_BITBOARD_ROOK][BLACK] | bb[PIECE_TYPE_BITBOARD_ROOK][WHITE]);

		  ASSERT_LV3(((rookStepEffect(to) & sq)));
		  break;

	  default:
		  UNREACHABLE;
	  } else {
		  // DIRECT_MISC
		  ASSERT_LV3(!(bishopStepEffect(to) & sq));
		  ASSERT_LV3(!((rookStepEffect(to) & sq)));
	  }

	  attackers &= occupied;

	  // この駒が成れるなら、成りの値を返すほうが良いかも。
	  // ※　最後にこの地点に残る駒を返すべきなのか。相手が取る/取らないを選択するので。
	  return type_of(pos.piece_on(sq));
  }

} // namespace


/// Position::see() is a static exchange evaluator: It tries to estimate the
/// material gain or loss resulting from a move.

// Position::see()は静的交換評価器(SEE)である。これは、指し手による駒による得失の結果
// を見積ろうと試みる。

// 最初に動かす駒側の手番から見た値が返る。

// ※　SEEの解説についてはググれ。
//
// ある升での駒の取り合いの結果、どれくらい駒得/駒損するかを評価する。
// 最初に引数として、指し手mが与えられる。この指し手に対して、同金のように取り返され、さらに同歩成のように
// 取り返していき、最終的な結果(評価値のうちの駒割りの部分の増減)を返すのが本来のSEE。

// ただし、途中の手順では、同金とした場合と同金としない場合とで、(そのプレイヤーは自分が)得なほうを選択できるものとする。

// ※　KINGを敵の利きに移動させる手は非合法手なので、ここで与えられる指し手にはそのような指し手は含まないものとする。
// また、SEEの地点(to)の駒をKINGで取る手は含まれるが、そのKINGを取られることは考慮しなければならない。
// 最後になった駒による成りの上昇値は考えない。

// このseeの最終的な値が、vを以上になるかどうかを判定する。
// こういう設計にすることで早期にvを超えないことが確定した時点でreturn出来る。

bool Position::see_ge(Move m, Value v) const
{
	// null windowのときのαβ探索に似たアルゴリズムを用いる。

	// 少し無駄ではあるが、Stockfishの挙動をなるべく忠実に再現する。

	bool drop = is_drop(m);
	// 駒の移動元(駒打ちの場合は)、移動先
	// dropのときにはSQ_NBにしておくことで、pieces() ^ fromを無効化するhack
	Square from = drop ? SQ_NB : from_sq(m);
	Square to = move_to(m);

	// 次にtoの升で捕獲される駒
	// 成りなら成りを評価したほうが良い可能性があるが、このあとの取り合いで指し手の成りを評価していないので…。
	Piece nextVictim = drop ? move_dropped_piece(m) : type_of(piece_on(from));

	// 移動させる駒側のturnから始まるものとする。
	// 次に列挙すべきは、この駒を取れる敵の駒なので、相手番に。
	Color stm = ~color_of(moved_piece_after(m));

	// 取り合いにおける収支。取った駒の価値と取られた駒の価値の合計。
	Value balance = (Value)Eval::CapturePieceValue[piece_on(to)];

	// この時点でマイナスになっているので早期にリターン。
	if (balance < v)
		return false;

	// 王が取られる指し手は考慮しなくて良いので、これは取られないものとしてプラス収支であるから
	// この時点でリターンできる。
	if (nextVictim == KING)
		return true;

	balance -= (Value)Eval::CapturePieceValue[nextVictim];

	if (balance >= v)
		return true;

	// true if the opponent is to move and false if we are to move
	bool relativeStm = true;
	Bitboard stmAttackers;

	// 盤上の駒(取り合いしていくうちにここから駒が無くなっていく)
	// すでにfromとtoの駒は取られたはずなので消しておく。
	Bitboard occupied = pieces() ^ from ^ to;

	// すべてのattackerを列挙する。
	Bitboard attackers = attackers_to(to, occupied) & occupied;

	while (true)
	{
		stmAttackers = attackers & pieces(stm);

		// pinnersが元の升にいる限りにおいては、pinされた駒から王以外への移動は許さない。

		if (!(st->pinnersForKing[stm] & ~occupied))
			stmAttackers &= ~st->blockersForKing[stm];

		if (!stmAttackers)
			return relativeStm;

		// 次に価値の低い攻撃駒を調べて取り除く。

		nextVictim = (stm == BLACK)
			// 将棋だと香の利きが先後で違うのでmin_attacker()に手番も渡してやる必要がある。
			? min_attacker<BLACK>(*this, to, stmAttackers, occupied, attackers)
			: min_attacker<WHITE>(*this, to, stmAttackers, occupied, attackers);

		if (nextVictim == KING)
			return relativeStm == bool(attackers & pieces(~stm));

		balance = relativeStm ? balance + (Value)Eval::CapturePieceValue[nextVictim]
							  : balance - (Value)Eval::CapturePieceValue[nextVictim];
		relativeStm = !relativeStm;

		if (relativeStm == (balance >= v))
			return relativeStm;

		stm = ~stm;
	}
}

#endif // USE_SEE
