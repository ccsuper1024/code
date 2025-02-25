package main

import (
	"testing"
	"time"
)

func TestParallel_1(t *testing.T) {
	t.Parallel()
	time.Sleep(2 * time.Second)
}

func TestParallel_2(t *testing.T) {
	t.Parallel()
	time.Sleep(2 * time.Second)
}

func TestParallen_3(t *testing.T) {
	t.Parallel()
	time.Sleep(2 * time.Second)
}
