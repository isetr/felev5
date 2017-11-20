module Generics =

    type UNIT =
        | UNIT
    with
        static member op_Equality (_: UNIT, _: UNIT) = true  

        override this.ToString() = "" 

    type PAIR<'A, 'B> = 
        | PAIR of 'A * 'B
    with
        static member op_Equality (l, r) =
            match l, r with
            | PAIR (a, b), PAIR (c, d) -> a = c && b = d

        override this.ToString() =
            match this with
            | PAIR (a, b) -> "(" + a.ToString() + " " + b.ToString() + ")" 

    type EITHER<'A, 'B> =
        | RIGHT of 'A
        | LEFT of 'B
    with
        static member op_Equality (l, r) =
            match l, r with
            | RIGHT a, RIGHT b -> a = b
            | LEFT a, LEFT b -> a = b
            | _, _ -> false

        override this.ToString() =        
            match this with
            | RIGHT a -> a.ToString()
            | LEFT a -> a.ToString()
       

    type CON<'A> =
        | CON of string * 'A
    with
        static member op_Equality (l, r) =
            match l, r with
            | CON (_, a), CON (_, b) -> a = b

        override this.ToString() =
            match this with
            | CON (s, a) -> "(" + s + " " + a.ToString() + ")"        

    type Generic<'A, 'B> =
        | U of CON<UNIT>
        | P of CON<PAIR<'A, 'B>>
        | E of CON<EITHER<'A, 'B>>
    with    
        static member op_Equality (l, r) =
            match l, r with
            | U _, U _ -> true
            | P a, P b -> a = b
            | E a, E b -> a = b
            | _, _ -> false

        override this.ToString() =
            match this with
            | U u -> u.ToString()
            | P a -> a.ToString()
            | E a -> a.ToString()        

module Model =
    open Generics
    
    type List<'A> =
        | Nil
        | Cons of 'A * List<'A>    
    with
        static member FromList (a: List<'A>) =
            match a with
            | Nil -> U (CON ("Nil", UNIT))
            | Cons (x, xs) -> P (CON ("Cons", PAIR (x, xs)))
        
        static member op_Equality (l, r) = List.FromList l = List.FromList r

        override this.ToString() = (List.FromList this).ToString()

    type Tree<'A, 'B> =
        | Leaf of 'A
        | Node of 'B * Tree<'A, 'B> * Tree<'A, 'B>
    with
        static member FromTree (a: Tree<'A, 'B>) =
            match a with
            | Leaf a -> E (CON ("Leaf", RIGHT a))
            | Node (b, ltr, rtr) -> E (CON ("Node", LEFT (b, ltr, rtr)))

        static member op_Equality (l, r) = Tree.FromTree l = Tree.FromTree r

        override this.ToString() = (Tree.FromTree this).ToString()

module Program =
    open Model

    let l1 = Cons (1, Cons (2, Cons (3, Nil)))
    let l2 = Nil
    let l3 = Cons (2, Cons (1, Cons (3, Cons (4, Nil))))
    let l4 = Cons (2, Cons (1, Cons (3, Cons (4, Nil))))

    let t1 = Leaf 1
    let t2 = Node ('a', Leaf 1, Leaf 2)
    let t3 = Node ('a', Leaf 2, Leaf 1)
    let t4 = Node ('b', Node ('a', Leaf 1, Leaf 3), Leaf 2)
    let t5 = Node ('b', Node ('a', Leaf 1, Leaf 3), Leaf 2)

    let Lists = [l1; l2; l3; l4]
    let Trees = [t1; t2; t3; t4; t5]

    let TestList =
        [
            l1 = l1
            not (l1 = l2)
            not (l2 = l1)
            l3 = l4
            l4 = l3
            t1 = t1
            not (t1 = t2)
            not (t2 = t1)
            not (t2 = t3)
            not (t3 = t2)
            t4 = t5
            t5 = t4
        ]

    let prettyPrint l = l |> List.fold (fun s v -> s + v.ToString() + "\n" ) "" 

    [<EntryPoint>]
    let main argv =
        printfn "%A" TestList
        printfn "\n%s\n%s" (prettyPrint Lists) (prettyPrint Trees)
        0
