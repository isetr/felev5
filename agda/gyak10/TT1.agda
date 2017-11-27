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
trans {A}{a}{a'}{a''} w = J {A = A}{a}(λ a' w → a' ≡ a'' → a ≡ a'') (λ w → w) w 

cong  : {A B : Set}(f : A → B){a a' : A} → a ≡ a' → f a ≡ f a'
cong {A}{B} f {a}{a'} w = J {_}{A}{_} (λ a' w → f a ≡ f a') refl w

subst : {A : Set}(P : A → Set){a a' : A} → a ≡ a' → P a → P a'
subst {A} P {a}{a'} w = J {_}{A}{a} (λ a' w → P a → P a') (λ w → w) w

idr : {A : Set}{a a' : A}(p : a ≡ a') → trans p refl ≡ p
idr {A}{a}{a'} p = J {_}{A}{a} (λ a' p → trans p refl ≡ p) (refl {A = a ≡ a}{refl}) p

idl : {A : Set}{a a' : A}(p : a ≡ a') → trans refl p ≡ p
idl {A}{a}{a'} p = J {_}{A}{a} (λ a' p → trans refl p ≡ p) (refl) p

-- true ≠ false

true? : Bool → Set
true? = λ x → if[ _ ] x then ⊤ else ⊥

true≢false : (true ≡ false) → ⊥
true≢false = λ p → subst true? p tt

-- 0 ≠ 1

zero? : ℕ → Set
zero? = λ x → Indℕ _ ⊤ (λ n → ⊥) x

0≢1 : (zero ≡ suc zero) → ⊥
0≢1 = λ p → subst zero? p tt

------------------------------------------------------------------------------
-- decidability of equality
------------------------------------------------------------------------------

Decidable : ∀{i} → Set i → Set i
Decidable = λ A → A + ¬ A

≡Bool : Bool → Bool → Bool
≡Bool = λ a b → IF a THEN b ELSE not b

test≡Bool₁ : ≡Bool true true ≡ true
test≡Bool₁ = refl

test≡Bool₂ : ≡Bool false false ≡ true
test≡Bool₂ = refl

test≡Bool₃ : ≡Bool true false ≡ false
test≡Bool₃ = refl

test≡Bool₄ : ≡Bool false true ≡ false
test≡Bool₄ = refl

dec≡Bool : (b b' : Bool) → Decidable (b ≡ b')
dec≡Bool = λ a b → if[ (λ a → (a ≡ b) + (a ≡ b → ⊥)) ] a then if[ (λ b → (true ≡ b) + (true ≡ b → ⊥)) ] b then inj₁ refl else inj₂ true≢false else (if[ (λ b → (false ≡ b) + (false ≡ b → ⊥)) ] b then inj₂ (λ ft → true≢false (sym ft)) else inj₁ refl)

dec≡ℕ : (n n' : ℕ) → Decidable (n ≡ n')
dec≡ℕ = λ a b → Indℕ (λ a → (a ≡ b) + (a ≡ b → ⊥))
                                 (Indℕ (λ b → (zero ≡ b) + (zero ≡ b → ⊥)) (inj₁ refl) (λ n → inj₂ λ p → subst zero? p tt) b)
                                 (λ n → inj₂ λ p → subst zero? (sym p) (subst zero? p ({!!})))
                                 a

dec≡+ : ∀{i}{A B : Set i}
        (dec≡A : (a a' : A) → Decidable (a ≡ a'))
        (dec≡B : (b b' : B) → Decidable (b ≡ b'))
      → (w w' : A + B) → Decidable (w ≡ w')
dec≡+ = λ a b w w' → case (λ x → (x ≡ w') + (x ≡ w' → ⊥)) (λ f → inj₁ {!!}) (λ g → {!!}) w

------------------------------------------------------------------------------
-- lists
------------------------------------------------------------------------------

l1 : List Bool
l1 = true ∷l true ∷l true ∷l true ∷l true ∷l []l

l2 : List Bool
l2 = []l


lengthl : {A : Set} → List A → ℕ
lengthl = {!!}

mapsl : {A B : Set} → List (A → B) → List A → List B
mapsl = {!!}

headl : {A : Set} → List A → A
headl = {!!}

taill : {A : Set} → List A → List A
taill = {!!}

headl' : {A : Set}(xs : List A) → ¬ (lengthl xs ≡ zero) → A
headl' = {!!}

------------------------------------------------------------------------------
-- vectors
------------------------------------------------------------------------------

n5 : ℕ
n5 = suc (suc (suc (suc (suc zero))))

v1 : Vec Bool n5
v1 = true ∷ true ∷ true ∷ true ∷ true ∷ []

v2 : Vec Bool zero
v2 = []

maps : {n : ℕ}{A B : Set} → Vec (A → B) n → Vec A n → Vec B n
maps = {!!}

head : {n : ℕ}{A : Set} → Vec A (suc n) → A
head = {!!}

tail : {n : ℕ}{A : Set} → Vec A (suc n) → A
tail = {!!}
