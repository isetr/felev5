module Prop where

open import lib

------------------------------------------------------------------------------
-- Implication
------------------------------------------------------------------------------

-- A → B -- \->

-- A → B → C = A → (B → C)

--  x : A ⊢ p : B              f : A → B    a : A
-- ---------------intro        ------------------elim
-- λ x → p : A → B                   f a : B

l1 : X → X
l1 = λ x → x

l2 : X → Y → X
l2 = λ x → λ y → x

l3 : (X → Y) → X
l3 = λ f → {!!}

l4 : X → Y → Y
l4 = λ x → λ y → y

l5 : X → X → X
l5 = λ x → λ x1 → x

l5' : X → X → X
l5' = λ x x1 → x

l6 : (X → Y → Z) → (Y → X → Z)
l6 = λ f → λ x y → f y x

------------------------------------------------------------------------------
-- True
------------------------------------------------------------------------------

-- ⊤ -- \top

-- 
-- ------intro
-- tt : ⊤

l7 : ⊤
l7 = tt

l8 : X → ⊤
l8 = λ x → tt

------------------------------------------------------------------------------
-- False
------------------------------------------------------------------------------

-- ⊥ -- \bot

--     p : ⊥
-- -------------elim
-- abort C p : C

l9 : ⊥ → X
l9 = λ f → abort X f

l9' : (X → ⊥) → (X → Y)
l9' = λ f → λ x → abort Y (f x)

-- negation : ¬ A = A → ⊥ -- \neg

l10 : X → ¬ ¬ X
l10 = λ x → λ y → y x

l11 : ¬ ¬ (¬ ¬ X → X)
l11 = {!!}

l12 : ¬ ¬ ¬ X → ¬ X
l12 = {!!}

l13 : ¬ X → ¬ ¬ ¬ X
l13 = {!!}

------------------------------------------------------------------------------
-- And
------------------------------------------------------------------------------

-- _∧_ -- \and

-- p : A   q : B              p : A ∧ B              p : A ∧ B      
-- -------------intro        -----------elim1       -----------elim2
-- p , q : A ∧ B             proj₁ p : A            proj₂ p : B

l14 : (X → Y) ∧ (Y → Z) → (X → Z)
l14 = λ f → λ x → proj₂ f (proj₁ f x)

l15 : ((X ∧ Y) ∧ Z) → (X ∧ (Y ∧ Z))
l15 = λ f → proj₁ (proj₁ f) , (proj₂ (proj₁ f) , proj₂ f)

l16 : (X → (Y ∧ Z)) → ((X → Y) ∧ (X → Z))
l16 = λ f → (λ x → proj₁ (f x)) , (λ x → proj₂ (f x))

-- if and only if: A ↔ B = (A → B) ∧ (B → A)\<->

l19 : X ↔ X ∧ ⊤
l19 = (λ f → f , tt) , (λ x → proj₁ x)

l17 : ¬ ¬ ¬ X ↔ ¬ X
l17 = (λ f → λ f' → f (λ f'' → f'' f')) , (λ f → λ f' → f (abort X (f' f)))

l18 : ¬ (X ↔ ¬ X)
--l18 = λ f → (λ x →  (proj₁ f x) x) (abort X ((proj₁ f {!!}) {!!}))
l18 = λ f → proj₁ f (proj₂ f (λ x → proj₁ f x x)) (proj₂ f (λ x → proj₁ f x x))

------------------------------------------------------------------------------
-- Or
------------------------------------------------------------------------------

-- _∨_ -- \or

--      p : A                    q : B          
-- --------------intro1     --------------intro2
-- inj₁ p : A ∨ B           inj₂ q : A ∨ B      
--
-- p : A → C      q : B → C     r : A ∨ B
-- --------------------------------------elim
--     case C p q r : C

l21 : X ∨ X → X
l21 = λ w → case _ l1 l1 w

l22 : (X ∧ Y) ∨ Z → (X ∨ Z) ∧ (Y ∨ Z)
l22 = λ f → case _ (λ g → inj₁ (proj₁ g)) (λ z → inj₂ z) f , case _ (λ g → inj₁ (proj₂ g)) (λ z → inj₂ z) f

l20 : X ↔ X ∨ ⊥
l20 = (λ x → inj₁ x) , (λ f → case X l1 (abort X) f)

l23 : (Y ∨ X) ↔ (X ∨ Y)
l23 = (λ f → case _ (λ y → inj₂ y) (λ x → inj₁ x) f) , (λ f → case _ (λ x → inj₂ x) (λ y → inj₁ y) f)

l24 : (X ∨ Y) → Z → (X → Z) ∧ (Y → Z)
l24 = λ f  z → (λ x → z) , (λ y → z)

l25 : ¬ ¬ (X ∨ ¬ X)
l25 = λ f → f (inj₂ (λ x → f (inj₁ x)))

l26 : (¬ X ∨ Y) → X → Y
l26 = λ f → λ x → case Y (λ nx → abort Y (nx x)) (λ y → y) f

l27 : ¬ (X ∨ Y) ↔ (¬ X ∧ ¬ Y)
l27 = (λ f → (λ x → f (inj₁ x)) , (λ y → f (inj₂ y))) , (λ f → λ g → proj₁ f (case _ l1 (λ y → abort X (proj₂ f y)) g))

l28 : (¬ X ∨ ¬ Y) → ¬ (X ∧ Y)
l28 = λ f → λ g → (case (X → ⊥) (λ nx → nx) (λ ny → λ x → abort _ (ny (proj₂ g))) f) (proj₁ g)
