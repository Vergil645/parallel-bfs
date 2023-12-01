#![allow(invalid_reference_casting)]

use super::graph::DiGraph;
use rayon::prelude::*;
use std::{
    collections::VecDeque,
    sync::atomic::{AtomicBool, Ordering},
};

pub fn sequential<G>(digraph: &G, src_vertex: usize) -> Vec<Option<usize>>
where
    G: DiGraph,
{
    let mut dist =
        Vec::from_iter((0..digraph.size()).map(|u| if u == src_vertex { Some(0) } else { None }));
    let mut queue = VecDeque::from_iter([src_vertex]);

    while let Some(u) = queue.pop_front() {
        for v in digraph.output_neigh(u) {
            if let None = dist[v] {
                dist[v] = dist[u].map(|d| d + 1);
                queue.push_back(v);
            }
        }
    }

    dist
}

pub fn parallel<G>(digraph: &G, src_vertex: usize) -> Vec<Option<usize>>
where
    G: DiGraph,
    for<'a> &'a G: Sync + Send,
{
    let reached = Vec::from_iter((0..digraph.size()).map(|u| AtomicBool::new(u == src_vertex)));
    let mut dist =
        Vec::from_iter((0..digraph.size()).map(|u| if u == src_vertex { Some(0) } else { None }));
    let mut frontier = vec![src_vertex];

    for cur_dist in 1.. {
        if frontier.is_empty() {
            break;
        }

        frontier = frontier
            .into_par_iter()
            .map(|u| digraph.output_neigh(u))
            .flat_map(|neigh_vec| {
                neigh_vec
                    .into_iter()
                    .flat_map(|v| {
                        if !reached[v].load(Ordering::Relaxed)
                            && reached[v]
                                .compare_exchange(false, true, Ordering::Relaxed, Ordering::Relaxed)
                                .is_ok()
                        {
                            // dist_tmp[v].store(cur_dist, Ordering::Relaxed);
                            // unsafe {
                            //     *(&dist[v] as *const _ as *mut _) = Some(cur_dist);
                            // }
                            dist[v] = Some(cur_dist);
                            Some(v)
                        } else {
                            None
                        }
                    })
                    .collect::<Vec<usize>>()
            })
            .collect();
    }

    dist
}
