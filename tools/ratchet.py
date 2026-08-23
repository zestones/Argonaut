"""Shared JSON-baseline helpers for the repository quality ratchets."""

import json
import os


def load_baseline(path):
    """Load a ratchet baseline, treating a missing file as an empty baseline."""
    if not os.path.exists(path):
        return {}
    with open(path, encoding="utf-8") as baseline_file:
        return json.load(baseline_file)


def write_baseline(path, counts):
    """Write deterministic, human-readable ratchet counters."""
    with open(path, "w", encoding="utf-8") as baseline_file:
        json.dump(counts, baseline_file, indent=2, sort_keys=True)
        baseline_file.write("\n")
