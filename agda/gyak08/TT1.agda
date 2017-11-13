{-# OPTIONS --rewriting #-}

module TT1 where

open import tt

------------------------------------------------------------------------------
-- empty
------------------------------------------------------------------------------

emptyExample : (A B : Set) → (A → ⊥) → A → B
emptyExample A B na a = abort {C = B} (na a)

------------------------------------------------------------------------------
-- booleans
------------------------------------------------------------------------------

IF_THEN_ELSE_ : Bool → {C : Set} → C → C → C
IF b THEN t ELSE f = if[ _ ] b then t else f

IFtrue : ∀{C}{t f : C} → IF true THEN t ELSE f ≡ t
IFtrue = refl

IFfalse : ∀{C}{t f : C} → IF false THEN t ELSE f ≡ f
IFfalse = refl

not : Bool → Bool
not = λ b → IF b THEN false ELSE true

nottrue : not true ≡ false
nottrue = refl

notfalse : not false ≡ true
notfalse = refl

and : Bool → Bool → Bool
and = λ a b → IF a THEN b ELSE false

and-lid : ∀ b → and true b ≡ b
and-lid = λ _ → refl

and-rid : ∀ b → and b true ≡ b
and-rid = λ b →  if[ (λ b → and b true ≡ b)  ] b then refl else refl

or : Bool → Bool → Bool
or = λ a b → IF a THEN true ELSE b

or-lid : ∀ b → or false b ≡ b
or-lid = λ _ → refl

or-rid : ∀ b → or b false ≡ b
or-rid = λ b → if[ (λ b → or b false ≡ b) ] b then refl else refl

xor : Bool → Bool → Bool
xor = λ a b → IF a THEN not b ELSE b

and-comm : ∀ b b' → and b b' ≡ and b' b
and-comm = λ a b → if[ (λ a → and a b ≡ and b a) ] a then if[ (λ b → and true b ≡ and b true) ] b then refl else refl else (if[ (λ b → and false b ≡ and b false) ] b then refl else refl) 

or-comm : ∀ b b' → or b b' ≡ or b' b
or-comm = λ a b → if[ (λ a → or a b ≡ or b a) ] a then if[ (λ b → or true b ≡ or b true) ] b then refl else refl else (if[ (λ b → or false b ≡ or b false) ] b then refl else refl)

xor-comm : ∀ b b' → xor b b' ≡ xor b' b
xor-comm = λ a b → if[ (λ a → xor a b ≡ xor b a) ] a then if[ (λ b → xor true b ≡ xor b true) ] b then refl else refl else (if[ (λ b → xor false b ≡ xor b false) ] b then refl else refl)

------------------------------------------------------------------------------
-- equality
------------------------------------------------------------------------------

sym   : {A : Set}{a a' : A} → a ≡ a' → a' ≡ a
sym {A}{a}{a'} w = J {A = A}{a}(λ a' w → a' ≡ a) refl {a'} w   

trans : {A : Set}{a a' a'' : A} → a ≡ a' → a' ≡ a'' → a ≡ a''
trans {A}{a}{a'}{a''} w = J {A = A}{a}(λ a' w → a' ≡ a'' → a ≡ a'') λ w → w w 

cong  : {A B : Set}(f : A → B){a a' : A} → a ≡ a' → f a ≡ f a'
cong = {!!}

subst : {A : Set}(P : A → Set){a a' : A} → a ≡ a' → P a → P a'
subst = {!!}

-- true ≠ false

true? : Bool → Set
true? = {!!}

true≢false : (true ≡ false) → ⊥
true≢false = {!!}

-- 0 ≠ 1

zero? : ℕ → Set
zero? = {!!}

0≢1 : (zero ≡ suc zero) → ⊥
0≢1 = {!!}

------------------------------------------------------------------------------
-- decidability of equality
------------------------------------------------------------------------------

Decidable : ∀{i} → Set i → Set i
Decidable = λ A → A + ¬ A

≡Bool : Bool → Bool → Bool
≡Bool = {!!}

test≡Bool₁ : ≡Bool true true ≡ true
test≡Bool₁ = {!!}

test≡Bool₂ : ≡Bool false false ≡ true
test≡Bool₂ = {!!}

test≡Bool₃ : ≡Bool true false ≡ false
test≡Bool₃ = {!!}

test≡Bool₄ : ≡Bool false true ≡ false
test≡Bool₄ = {!!}

dec≡Bool : (b b' : Bool) → Decidable (b ≡ b')
dec≡Bool = {!!}

dec≡ℕ : (n n' : ℕ) → Decidable (n ≡ n')
dec≡ℕ = {!!}

dec≡+ : ∀{i}{A B : Set i}
        (dec≡A : (a a' : A) → Decidable (a ≡ a'))
        (dec≡B : (b b' : B) → Decidable (b ≡ b'))
      → (w w' : A + B) → Decidable (w ≡ w')
dec≡+ = {!!}
