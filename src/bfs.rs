#![allow(invalid_reference_casting)]

use super::graph::DiGraph;
use rayon::prelude::*;
use std::{
    collections::VecDeque,
    sync::atomic::{AtomicBool, AtomicUsize, Ordering},
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
    let reached = Vec::from_iter((0..digraph.size()).map(|u| {
        if u == src_vertex {
            AtomicBool::new(true)
        } else {
            AtomicBool::new(false)
        }
    }));
    let dist_tmp = Vec::from_iter((0..digraph.size()).map(|_| AtomicUsize::new(0)));
    let mut frontier = vec![src_vertex];
    for cur_dist in 1.. {
        if frontier.is_empty() {
            break;
        }

        frontier = frontier
            .par_iter()
            .map(|u_ref| digraph.output_neigh(*u_ref))
            .flatten()
            .flat_map(|v| {
                if let Ok(_) =
                    reached[v].compare_exchange(false, true, Ordering::Relaxed, Ordering::Relaxed)
                {
                    dist_tmp[v].store(cur_dist, Ordering::Relaxed);
                    Some(v)
                } else {
                    None
                }
            })
            .collect();
    }

    dist_tmp
        .into_par_iter()
        .zip(reached.into_par_iter())
        .map(|(d, is_r)| {
            if is_r.load(Ordering::Relaxed) {
                Some(d.load(Ordering::Relaxed))
            } else {
                None
            }
        })
        .collect()
}
