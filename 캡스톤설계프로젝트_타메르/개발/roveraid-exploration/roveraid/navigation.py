"""Navigation boundary: mission code never calls GPIO or serial APIs."""
from abc import ABC, abstractmethod


class NavigationAdapter(ABC):
    @abstractmethod
    def goto_cell(self, cell):
        """Request a goal in the current map's cell coordinate system."""

    @abstractmethod
    def cancel(self):
        """Cancel the active navigation task."""


class MockNavigation(NavigationAdapter):
    def __init__(self, mock_map):
        self.map = mock_map

    def goto_cell(self, cell):
        return self.map.navigate(cell)

    def cancel(self):
        self.map.cancel()

    def step(self):
        return self.map.step()
