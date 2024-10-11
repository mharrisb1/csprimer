use std::{thread::sleep, time::Duration};

use rand::random;

const ALIVE_TOK: &str = "██";
const DEAD_TOK: &str = "░░";

fn main() {
    let w = 50;
    let h = 50;
    let interval_ms = 100;
    let mut iterations = 1000;

    let mut grid = Grid::new(w, h);
    let mut grid_next = grid.clone();

    grid.seed();

    while iterations > 0 {
        clear_term();
        grid.display();
        grid.transition(&mut grid_next);
        grid.swap(&mut grid_next);
        iterations -= 1;
        sleep(Duration::from_millis(interval_ms));
    }
}

fn clear_term() {
    print!("{}[2J", 27 as char);
}

#[derive(Clone)]
struct Grid {
    width: i32,
    height: i32,
    cells: Vec<bool>,
}

impl Grid {
    fn new(width: i32, height: i32) -> Self {
        let size = width * height;
        Self {
            width,
            height,
            cells: vec![false; size as usize],
        }
    }

    fn swap(&mut self, other: &mut Grid) {
        std::mem::swap(self, other);
    }

    fn seed(&mut self) {
        let size = self.width * self.height;
        for i in 0..size {
            self.cells[i as usize] = random();
        }
    }

    fn transition(&self, next: &mut Grid) {
        for y in 0..self.height {
            for x in 0..self.width {
                let k = self.get_cell_neighbor_count(x, y);
                match self.get_cell(x, y) {
                    true => next.set_cell(x, y, (2..=3).contains(&k)),
                    false => next.set_cell(x, y, k == 3),
                }
            }
        }
    }

    fn display(&self) {
        for y in 0..self.height {
            for x in 0..self.width {
                match self.get_cell(x, y) {
                    true => print!("{}", ALIVE_TOK),
                    false => print!("{}", DEAD_TOK),
                }
            }
            println!();
        }
    }

    fn get_cell(&self, x: i32, y: i32) -> bool {
        self.cells[(x + self.width * y) as usize]
    }

    fn set_cell(&mut self, x: i32, y: i32, val: bool) {
        self.cells[(x + self.width * y) as usize] = val;
    }

    fn get_cell_neighbor_count(&self, x: i32, y: i32) -> u8 {
        let mut k = 0u8;
        let i = x + self.width * y;

        let deltas = [-1, 0, 1];

        for dy in deltas.iter() {
            for dx in deltas.iter() {
                if *dy == 0 && *dx == 0 {
                    continue; // don't include cell
                }
                if i < self.width && *dy < 0 {
                    continue; // can't look up
                }
                if i % self.width == 0 && *dx < 0 {
                    continue; // can't look left
                }
                if (i + 1) % self.width == 0 && *dx > 0 {
                    continue; // can't look right
                }
                if i >= ((self.height - 1) * self.width) && *dy > 0 {
                    continue; // can't look down
                }
                k += self.cells[(i + (dy * self.width + dx)) as usize] as u8;
            }
        }

        k
    }
}
