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

IF_THEN_ELSE_ : ∀{i} → Bool → {C : Set i} → C → C → C
IF b THEN t ELSE f = if[ _ ] b then t else f

IFtrue : ∀{i}{C : Set i}{t f : C} → IF true THEN t ELSE f ≡ t
IFtrue = refl

IFfalse : ∀{i}{C : Set i}{t f : C} → IF false THEN t ELSE f ≡ f
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
and-rid = λ b → {!!}

or : Bool → Bool → Bool
or = {!!}

xor : Bool → Bool → Bool
xor = {!!}

and-comm : ∀ b b' → and b b' ≡ and b' b
and-comm = {!!}

or-comm : ∀ b b' → or b b' ≡ or b' b
or-comm = {!!}

xor-comm : ∀ b b' → xor b b' ≡ xor b' b
xor-comm = {!!}

------------------------------------------------------------------------------
-- equality
------------------------------------------------------------------------------

sym   : ∀{i}{A : Set i}{a a' : A} → a ≡ a' → a' ≡ a
sym = {!!}

trans : ∀{i}{A : Set i}{a a' a'' : A} → a ≡ a' → a' ≡ a'' → a ≡ a''
trans = {!!}

cong  : ∀{i j}{A : Set i}{B : Set j}(f : A → B){a a' : A} → a ≡ a' → f a ≡ f a'
cong = {!!}

subst : ∀{i j}{A : Set i}(P : A → Set j){a a' : A} → a ≡ a' → P a → P a'
subst = {!!}

idr : {A : Set}{a a' : A}(p : a ≡ a') → trans p refl ≡ p
idr = {!!}

idl : {A : Set}{a a' : A}(p : a ≡ a') → trans refl p ≡ p
idl = {!!}

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

------------------------------------------------------------------------------
-- isomorphism of Bool and ⊤ + ⊤
------------------------------------------------------------------------------

_≅_ : Set → Set → Set -- \~=
A ≅ B = Σ (A → B) λ f → Σ (B → A) λ g → ((x : A) → g (f x) ≡ x) × ((y : B) → f (g y) ≡ y)

w : Bool ≅ (⊤ + ⊤)
w =  f
  , (g
  , ((λ x → if[ (λ x → g (f x) ≡ x) ] x then refl else refl)
  ,  λ y → case (λ y → f (g y) ≡ y) (Ind⊤ _ refl) (Ind⊤ _ refl) y))
  where
    f : Bool → ⊤ + ⊤
    f = λ b → if[ (λ _ → ⊤ + ⊤) ] b then inj₁ tt else inj₂ tt

    g : ⊤ + ⊤ → Bool
    g = λ u → case (λ _ → Bool) (λ _ → true) (λ _ → false) u

------------------------------------------------------------------------------
-- the two different isomorphisms between Bool and Bool
------------------------------------------------------------------------------

idiso : Bool ≅ Bool
idiso = (λ x → x ) , ((λ x → x) , ((λ x → if[ (λ x → x ≡ x) ] x then refl else refl) , ((λ x → if[ (λ x → x ≡ x) ] x then refl else refl)))) 

notiso : Bool ≅ Bool
notiso = not , (not , ((λ x → if[ (λ x → not (not x) ≡ x) ] x then refl else refl) , (λ x → if[ (λ x → not (not x) ≡ x) ] x then refl else refl)))

------------------------------------------------------------------------------
-- Curry - Uncurry
------------------------------------------------------------------------------

curryiso : ∀{A B C : Set} → (A → B → C) ≅ (A × B → C)
curryiso = (λ f → (λ ab → f (proj₁ ab) (proj₂ ab))) ,
                ((λ f → λ a b → f (a , b)) ,
                ((λ x → funext (λ w → refl)) ,
                (λ y → funext (λ x → IndΣ (λ xy → y (proj₁ xy , proj₂ xy) ≡ y x) (λ a b → (cong y {!(xprojs ?)!})) x))))
    where
      xprojs : ∀{i j}{A : Set i}{B : A → Set j}(w : Σ A B) → (proj₁ w , proj₂ w) ≡ w
      xprojs {A = A}{B} = IndΣ (λ w → proj₁ w , proj₂ w ≡ w) (λ a b → refl) 

is : ∀{A : Set} → (Bool → A) ≅ (A × A)
is = (λ f → f true ,  f false) ,
       ((λ aa → λ b → if[ _ ] b then proj₁ aa else proj₂ aa) ,
       ((λ x → funext (λ f → {!!})) ,
       (λ aa → {!!})))

------------------------------------------------------------------------------
-- random exercises
------------------------------------------------------------------------------

congid : ∀{ℓ}{A : Set ℓ}{x y : A}(p : x ≡ y) → cong (λ x → x) p ≡ p
congid = {!!}

≡inv : ∀{ℓ}{A : Set ℓ} {x y : A} (p : x ≡ y) → (trans p (sym p)) ≡ refl
≡inv = {!!}

,=0 : ∀{ℓ ℓ'}{A : Set ℓ}{B : A → Set ℓ'}{a a' : A}{b : B a}{b' : B a'}
   → _≡_ {A = Σ A B} (a , b) (a' , b') → a ≡ a'
,=0 = {!!}

,=1 : ∀{ℓ ℓ'}{A : Set ℓ}{B : A → Set ℓ'}{a a' : A}{b : B a}{b' : B a'}
      (p : (a , b) ≡ (a' , b')) → subst B (,=0 p) b ≡ b'
,=1 = {!!}

,= : ∀{ℓ ℓ'}{A : Set ℓ}{B : A → Set ℓ'}{a a' : A}{b : B a}{b' : B a'}
     (p : a ≡ a') → subst B p b ≡ b' → _≡_ {A = Σ A B} (a , b) (a' , b')
,= = {!!}

-- TODO: set → h1
-- TODO: Hedberg
