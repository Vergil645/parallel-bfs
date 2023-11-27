pub trait DiGraph {
    fn size(&self) -> usize;
    fn output_neigh(&self, vertex: usize) -> Vec<usize>;
}