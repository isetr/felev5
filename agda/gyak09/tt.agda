{-# OPTIONS --rewriting #-}

module tt where

open import Agda.Primitive

infixl 4 _,_
infixl 4 _+_
infixl 3 _↔_
infix 3 _≡_
infixl 5 _×_

------------------------------------------------------------------------------
-- function space
------------------------------------------------------------------------------

-- builtin

-- A : Set      x : A ⊢ B : Set
-- ----------------------------type formation
--      (x : A) → B : Set

--     x : A ⊢ t : B
-- ---------------------introduction
-- λ x → t : (x : A) → B

-- f : (x : A) → B    u : A
-- ------------------------elimination
--      f u : B[x ↦ u]

-- x : A ⊢ t : B    u : A
-- ----------------------computation (β)
-- (λ x → t) u = t[x ↦ u]

-- f : (x : A) → B
-- ---------------uniqueness
--  λ x → f x = f

------------------------------------------------------------------------------
-- equality
------------------------------------------------------------------------------

postulate
  _≡_   : ∀{i}{A : Set i} → A → A → Set i
  refl  : ∀{i}{A : Set i}{a : A} → a ≡ a
  J     : ∀{i}{A : Set i}{a : A}{j}(P : (x : A) → a ≡ x → Set j) → P a refl → {a' : A}(w : a ≡ a') → P a' w
  reflβ : ∀{i}{A : Set i}{a : A}{j}{P : (x : A) → a ≡ x → Set j}{pr : P a refl} → J P pr refl ≡ pr

{-# BUILTIN REWRITE _≡_ #-}
{-# REWRITE reflβ #-}

------------------------------------------------------------------------------
-- empty type
------------------------------------------------------------------------------

postulate
  ⊥ : Set
  abort : ∀{i}{C : Set i} → ⊥ → C

------------------------------------------------------------------------------
-- one element type
------------------------------------------------------------------------------

postulate
  ⊤ : Set
  tt : ⊤

------------------------------------------------------------------------------
-- sum type
------------------------------------------------------------------------------

postulate
  _+_   : ∀{i}(A B : Set i) → Set i
  inj₁  : ∀{i}{A B : Set i} → A → A + B
  inj₂  : ∀{i}{A B : Set i} → B → A + B
  case  : ∀{i}{A B : Set i}{j}(P : A + B → Set j)(f : (x : A) → P (inj₁ x))(g : (x : B) → P (inj₂ x))(w : A + B) → P w
  inj₁β : ∀{i}{A B : Set i}{j}{P : A + B → Set j}{f : (x : A) → P (inj₁ x)}{g : (x : B) → P (inj₂ x)}{a : A} → case P f g (inj₁ a) ≡ f a
  inj₂β : ∀{i}{A B : Set i}{j}{P : A + B → Set j}{f : (x : A) → P (inj₁ x)}{g : (x : B) → P (inj₂ x)}{b : B} → case P f g (inj₂ b) ≡ g b
  {-# REWRITE inj₁β #-}
  {-# REWRITE inj₂β #-}

-- negation
¬_ : ∀{i} → Set i → Set i
¬ A = A → ⊥

------------------------------------------------------------------------------
-- Sigma type
------------------------------------------------------------------------------

postulate
  Σ      : ∀{i j}(A : Set i)(B : A → Set j) → Set (i ⊔ j)
  _,_    : ∀{i j}{A : Set i}{B : A → Set j}(a : A) → B a → Σ A B
  proj₁  : ∀{i j}{A : Set i}{B : A → Set j}(w : Σ A B) → A
  proj₂  : ∀{i j}{A : Set i}{B : A → Set j}(w : Σ A B) → B (proj₁ w)
  proj₁β : ∀{i j}{A : Set i}{B : A → Set j}{a : A}{b : B a} → proj₁ {B = B} (a , b) ≡ a
  {-# REWRITE proj₁β #-}
  proj₂β : ∀{i j}{A : Set i}{B : A → Set j}{a : A}{b : B a} → proj₂ {B = B} (a , b) ≡ b
  {-# REWRITE proj₂β #-}

-- Cartesian product
_×_ : ∀{i j} → Set i → Set j → Set (i ⊔ j)
A × B = Σ A λ _ → B

-- logical equivalence
_↔_ : ∀{i j} → Set i → Set j → Set (i ⊔ j)
A ↔ B = (A → B) × (B → A)

------------------------------------------------------------------------------
-- natural numbers
------------------------------------------------------------------------------

postulate
  ℕ     : Set
  zero  : ℕ
  suc   : ℕ → ℕ
  Indℕ  : ∀{i}(P : ℕ → Set i)(pz : P zero)(ps : {n : ℕ} → P n → P (suc n))(n : ℕ) → P n
  zeroβ : ∀{i}{P : ℕ → Set i}{pz : P zero}{ps : {n : ℕ} → P n → P (suc n)} → Indℕ P pz ps zero ≡ pz
  sucβ  : ∀{i}{P : ℕ → Set i}{pz : P zero}{ps : {n : ℕ} → P n → P (suc n)}{n : ℕ} → Indℕ P pz ps (suc n) ≡ ps (Indℕ P pz ps n)
  {-# REWRITE zeroβ #-}
  {-# REWRITE sucβ #-}

------------------------------------------------------------------------------
-- booleans
------------------------------------------------------------------------------

postulate
  Bool     : Set
  true  : Bool
  false : Bool
  if[_]_then_else_ : ∀{i}(P : Bool → Set i)(b : Bool)(pt : P true)(pf : P false) → P b
  trueβ            : ∀{i}{P : Bool → Set i}{pt : P true}{pf : P false} → if[ P ] true then pt else pf ≡ pt
  falseβ           : ∀{i}{P : Bool → Set i}{pt : P true}{pf : P false} → if[ P ] false then pt else pf ≡ pf
  {-# REWRITE trueβ #-}
  {-# REWRITE falseβ #-}

------------------------------------------------------------------------------
-- lists
------------------------------------------------------------------------------

postulate
  List    : ∀{i}(A : Set i) → Set i
  []l     : ∀{i}{A : Set i} → List A
  _∷l_    : ∀{i}{A : Set i}(x : A)(xs : List A) → List A
  IndList : ∀{i}{A : Set i}{j}(P : List A → Set j)(p[] : P []l)(p∷ : {xs : List A} → P xs → (x : A) → P (x ∷l xs))
          → (xs : List A) → P xs
  []lβ    : ∀{i}{A : Set i}{j}{P : List A → Set j}{p[] : P []l}{p∷ : {xs : List A} → P xs → (x : A) → P (x ∷l xs)}
          → IndList P p[] p∷ []l ≡ p[]
  ∷lβ     : ∀{i}{A : Set i}{j}{P : List A → Set j}{p[] : P []l}{p∷ : {xs : List A} → P xs → (x : A) → P (x ∷l xs)}
          → {xs : List A}{x : A} → IndList P p[] p∷ (x ∷l xs) ≡ p∷ (IndList P p[] p∷ xs) x

infixr 5 _∷l_

------------------------------------------------------------------------------
-- vectors
------------------------------------------------------------------------------

postulate
  Vec    : ∀{i}(A : Set i) → ℕ → Set i
  []     : ∀{i}{A : Set i} → Vec A zero
  _∷_    : ∀{i}{A : Set i}(x : A){n : ℕ}(xs : Vec A n) → Vec A (suc n)
  IndVec : ∀{i}{A : Set i}{j}(P : {n : ℕ} → Vec A n → Set j)(p[] : P [])(p∷ : {n : ℕ}{xs : Vec A n} → P xs → (x : A) → P (x ∷ xs))
         → {n : ℕ}(xs : Vec A n) → P xs
  []β    : ∀{i}{A : Set i}{j}{P : {n : ℕ} → Vec A n → Set j}{p[] : P []}{p∷ : {n : ℕ}{xs : Vec A n} → P xs → (x : A) → P (x ∷ xs)}
         → IndVec P p[] p∷ [] ≡ p[]
  ∷β     : ∀{i}{A : Set i}{j}{P : {n : ℕ} → Vec A n → Set j}{p[] : P []}{p∷ : {n : ℕ}{xs : Vec A n} → P xs → (x : A) → P (x ∷ xs)}
           {n : ℕ}{xs : Vec A n}{x : A} → IndVec P p[] p∷ (x ∷ xs) ≡ p∷ (IndVec P p[] p∷ xs) x

infixr 5 _∷_
