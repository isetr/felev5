module Generics =

    [<CustomEquality; NoComparison>]
    type UNIT =
        | UNIT
    with    
        override this.ToString() = "" 

        override l.Equals r = 
            match r with
            | :? UNIT -> true
            | _ -> false

        override this.GetHashCode() = 0

    [<CustomEquality; NoComparison>]
    type PAIR<'A, 'B when 'A : equality and 'B : equality> = 
        | PAIR of 'A * 'B
    with
        override p.ToString() =
            match p with
            | PAIR (a, b) -> "(" + a.ToString() + " " + b.ToString() + ")" 

        override l.Equals r =
            match r with
            | :? PAIR<'A, 'B> as r ->
                match l, r with
                | PAIR (a, b), PAIR (c, d) -> a = c && b = d
            | _ -> false

        override this.GetHashCode() =
            match this with
            | PAIR (a, b) -> a.GetHashCode() + b.GetHashCode() 

    [<CustomEquality; NoComparison>]
    type EITHER<'A, 'B when 'A : equality and 'B : equality> =
        | RIGHT of 'A
        | LEFT of 'B
    with
        override e.ToString() =        
            match e with
            | RIGHT a -> a.ToString()
            | LEFT a -> a.ToString()

        override l.Equals r =
            match r with
            | :? EITHER<'A, 'B> as r ->
                match l, r with
                | RIGHT a, RIGHT b -> a = b
                | LEFT a, LEFT b -> a = b
                | _, _ -> false
            | _ -> false

        override this.GetHashCode() =
            match this with
            | RIGHT a -> a.GetHashCode()
            | LEFT a -> a.GetHashCode()        

    [<CustomEquality; NoComparison>]
    type CON<'A when 'A : equality> =
        | CON of string * 'A
    with
        override c.ToString() =
            match c with
            | CON (s, a) -> "(" + s + " " + a.ToString() + ")"

        override l.Equals r =
            match r with
            | :? CON<'A> as r ->
                match l, r with
                | CON (_, a), CON (_, b) -> a = b
            | _ -> false

        override this.GetHashCode() =
            match this with
            | CON (s, a) -> s.GetHashCode() + a.GetHashCode()        

    [<CustomEquality; NoComparison>]
    type Generic<'A, 'B when 'A : equality and 'B : equality> =
        | U of CON<UNIT>
        | P of CON<PAIR<'A, 'B>>
        | E of CON<EITHER<'A, 'B>>
    with    
        override this.ToString() =
            match this with
            | U u -> u.ToString()
            | P a -> a.ToString()
            | E a -> a.ToString()
     
        override l.Equals r =
            match r with
            | :? Generic<'A, 'B> as r ->
                match l, r with
                | U _, U _ -> true
                | P (CON (_, a)), P (CON (_, b)) -> PAIR.Equals (a, b)
                | E (CON (_, a)), E (CON (_, b)) -> EITHER.Equals (a, b)
                | _, _ -> false
            | _ -> false

        override this.GetHashCode() =
            match this with
            | U u -> u.GetHashCode()
            | P p -> p.GetHashCode()
            | E e -> e.GetHashCode()      

module Model =
    open Generics
    
    [<CustomEquality; NoComparison>]
    type List<'A when 'A : equality> =
        | Nil
        | Cons of 'A * List<'A>    
    with
        static member FromList (a: List<'A>) =
            match a with
            | Nil -> U (CON ("Nil", UNIT))
            | Cons (x, xs) -> P (CON ("Cons", PAIR (x, xs)))
        
        override l.Equals r =
            match r with
            | :? List<'A> as r -> List.FromList l = List.FromList r
            | _ -> false

        override l.GetHashCode() = (List.FromList l).GetHashCode()

        override this.ToString() = (List.FromList this).ToString()

    [<CustomEquality; NoComparison>]
    type Tree<'A, 'B  when 'A : equality and 'B : equality> =
        | Leaf of 'A
        | Node of 'B * Tree<'A, 'B> * Tree<'A, 'B>
    with    
        static member FromTree (a: Tree<'A, 'B>) =
            match a with
            | Leaf a -> E (CON ("Leaf", RIGHT a))
            | Node (b, ltr, rtr) -> E (CON ("Node", LEFT (b, ltr, rtr)))

        override l.Equals r = 
            match r with
            | :? Tree<'A, 'B> as r -> Tree.FromTree l = Tree.FromTree r
            | _ -> false

        override t.GetHashCode() = (Tree.FromTree t).GetHashCode()        
            
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
        printfn "\n%s" (
            if List.forall id TestList then
                "Every test passed. Custom Equality is working."
            else
                "Custom Equality tests failed: \n" + 
                (
                    TestList
                    |> List.mapi (fun i b -> sprintf "Test #%d" i, b)
                    |> List.filter (snd >> not)
                    |> List.map fst
                    |> List.reduce (fun l r -> l + "\n" + r)
                )    
        )
        printfn "\n%s\n%s" (prettyPrint Lists) (prettyPrint Trees)
        0
