use cw_2::{bfs, graph::DiGraph};

struct CubeDiGraph {
    edge: usize,
}

impl CubeDiGraph {
    fn new(edge: usize) -> Self {
        Self { edge }
    }

    fn vertex_to_cube_node(&self, vertex: usize) -> (usize, usize, usize) {
        let x = vertex / (self.edge + 1).pow(2);
        let y = vertex % (self.edge + 1).pow(2) / (self.edge + 1);
        let z = vertex % (self.edge + 1);
        (x, y, z)
    }

    fn cube_node_to_vertex(&self, x: usize, y: usize, z: usize) -> usize {
        x * (self.edge + 1).pow(2) + y * (self.edge + 1) + z
    }
}

impl DiGraph for CubeDiGraph {
    fn size(&self) -> usize {
        (self.edge + 1).pow(3)
    }

    fn output_neigh(&self, vertex: usize) -> Vec<usize> {
        let (x, y, z) = self.vertex_to_cube_node(vertex);
        let mut neigh = Vec::new();

        if x < self.edge {
            neigh.push(self.cube_node_to_vertex(x + 1, y, z));
        }
        if y < self.edge {
            neigh.push(self.cube_node_to_vertex(x, y + 1, z));
        }
        if z < self.edge {
            neigh.push(self.cube_node_to_vertex(x, y, z + 1));
        }

        neigh
    }
}

fn main() {
    let digraph = CubeDiGraph::new(500);
    println!("1");
    let dist_seq = bfs::sequential(&digraph, 0);
    println!("2");
    let dist_par = rayon::ThreadPoolBuilder::new()
        .num_threads(4)
        .build()
        .map(|pool| pool.install(|| bfs::parallel(&digraph, 0)))
        .unwrap();
    println!("3");

    // println!("dist_seq={:?}", dist_seq);
    // println!("dist_par={:?}", dist_par);
    println!("{}", dist_seq == dist_par);
}
