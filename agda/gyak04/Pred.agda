module Pred where

open import lib

------------------------------------------------------------------------------
-- Forall
------------------------------------------------------------------------------

-- ∀(x : M) → P x   -- \forall

--     x : M ⊢ p : P x                  f : ∀(x : M) → P x      m : M
-- ------------------------intro        -----------------------------elim
-- λ x → p : ∀(x : M) → P x                       f m : P m

l1 : (∀(x : M) → P x ∧ Q x) ↔ (∀(x : M) → P x) ∧ (∀(x : M) → Q x)
l1 = (λ f → (λ x → proj₁ (f x)) , (λ x → proj₂ (f x))) , (λ f → λ x → proj₁ f x , proj₂ f x)

l2 : ¬ ¬ (∀(x : M) → P x) → ∀(x : M) → ¬ ¬ (P x)
l2 = λ f → λ x → λ nx → f (λ h → nx (h x))

------------------------------------------------------------------------------
-- Exists
------------------------------------------------------------------------------

-- ∃ M (λ x → P x)   -- \exists

-- m : M        p : P m           w : ∃ M λ x → P x        w : ∃ M λ x → P x
-- -----------------------intro   -----------------elim1  ---------------------elim2  
-- (m , p) : ∃ M λ x → P x          proj₁ w : M           proj₂ w : P (proj₁ m)           


l3 : (∃ M λ x → ¬ P x) → ¬ (∀(x : M) → P x)
l3 = λ f → λ g → proj₂' f (g (proj₁' f))

l4 : (¬ ∃ M λ x → P x) → ∀(x : M) → ¬ P x
l4 = λ f → λ g → λ np → f (g ,' np)

l5 : (∀(x : M) → ¬ P x) → ¬ ∃ M λ x → P x
l5 = λ f → λ g → (f (proj₁' g)) (proj₂' g )

l6 : (∃ M λ x → ∀(y : N) → R x y) → ∀(y : N) → ∃ M λ x → R x y
l6 = λ f → λ y → proj₁' f ,' proj₂' f y

-- axiom of choice

AC : (∀(x : M) → ∃ N λ y → R x y) → ∃ (M → N) λ f → ∀(x : M) → R x (f x)
AC = λ x → (λ m → proj₁' (x m)) ,' (λ g → proj₂' (x g))
