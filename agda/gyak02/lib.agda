{-# OPTIONS --rewriting #-}

module lib where

infixl 5 _∧_
infixl 4 _,_
infixl 4 _∨_
infixl 3 _↔_

postulate
  X Y Z : Set -- propositional variables
  
  ⊥ : Set
  abort : (C : Set) → ⊥ → C
  
  ⊤ : Set
  tt : ⊤
  
  _∧_ : (A B : Set) → Set
  _,_ : {A B : Set} → A → B → Set
  proj₁ : {A B : Set} → A ∧ B → A
  proj₂ : {A B : Set} → A ∧ B → B

  _∨_ : (A B : Set) → Set
  inj₁ : {A B : Set} → A → A ∨ B
  inj₂ : {A B : Set} → B → A ∨ B
  case : {A B : Set}(C : Set) → (A → C) → (B → C) → A ∨ B → C

¬_ : Set → Set
¬ A = A → ⊥

_↔_ : Set → Set → Set
A ↔ B = (A → B) ∧ (B → A)
