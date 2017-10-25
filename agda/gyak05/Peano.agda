{-# OPTIONS --rewriting #-}

module Peano where

open import lib

three : ℕ  -- \bn
three = suc (suc (suc zero))

seven : ℕ
seven = suc (suc (suc (suc three)))

-- 0 ↦ 1
-- 1 ↦ 3
-- 2 ↦ 5
-- 3 ↦ 7
-- 4 ↦ 9

2*_+1 : ℕ → ℕ
2*_+1 = λ n → Rec (suc zero)
                  (λ x → suc (suc x)) n
{-
  2* three 1+
= 2*_1+ three
= (λ n → Rec (suc zero) (λ x → suc (suc x)) n) three
  (λ x → t                                   ) u
  t[x ↦ u]
= Rec (suc zero) (λ x → suc (suc x)) three
= Rec (suc zero) (λ x → suc (suc x)) (suc (suc (suc zero)))
= (λ x → suc (suc x)) (Rec (suc zero) (λ x → suc (suc x)) (suc (suc zero)))
= suc (suc (Rec (suc zero) (λ x → suc (suc x)) (suc (suc zero))))
= suc (suc ((λ x → suc (suc x)) (Rec (suc zero) (λ x → suc (suc x)) (suc zero))))
= suc (suc ((λ x → suc (suc x)) (Rec (suc zero) (λ x → suc (suc x)) (suc zero))))
= suc (suc (suc (suc (Rec (suc zero) (λ x → suc (suc x)) (suc zero)))))
= suc (suc (suc (suc (suc (suc (Rec (suc zero) (λ x → suc (suc x)) zero))))))
= suc (suc (suc (suc (suc (suc (suc zero))))))
-}
test : 2* three +1 ≡ seven   -- \==
test = refl seven

four five : ℕ
four = suc three
five = suc four

3*_+5 : ℕ → ℕ
3*_+5 = λ n → Rec five (λ x → suc (suc (suc x))) n

ten twelve seventeen : ℕ
ten       = suc (suc (suc seven))
twelve    = suc (suc ten)
seventeen = suc (suc (suc (suc (suc (suc (suc ten))))))

test1 : 3* four +5 ≡ seventeen
test1 = refl seventeen

test2 : 3* zero +5 ≡ five
test2 = refl five
{-

0 + b = b
(suc a') + b = suc (a' + b)
-}
_+_ : ℕ → ℕ → ℕ
_+_ = λ a b → Rec b (λ a'+b → suc a'+b) a

infixl 5 _+_

test+ : three + four ≡ seven
test+ = refl seven

--                     Rec n (λ a'+b → suc a'+b) zero ≡ n
--                     n       ≡ n
leftunit : ∀(n : ℕ) → zero + n ≡ n
leftunit = λ n → refl n

--                     Rec zero (λ a'+b → suc a'+b) n
rightunit : ∀(n : ℕ) → n + zero ≡ n
rightunit = λ n → Ind (λ x → x + zero ≡ x) (refl zero) (λ x → λ p → cong suc p) n

assoc : ∀(a b c : ℕ) → (a + b) + c ≡ a + (b + c)
assoc = λ a b → Ind (λ z → a + b + z ≡ a + (b + z)) {!!} {!!}

_*_ : ℕ → ℕ → ℕ
_*_ = {!!}

infixl 6 _*_

test* : three * four ≡ twelve
test* = {!!}

sucright : ∀(a b : ℕ) → a + suc b ≡ suc (a + b)
sucright = {!!}

comm : ∀(a b : ℕ) → a + b ≡ b + a
comm = {!!}
