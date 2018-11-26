import javafx.application.Application;
import javafx.scene.control.Menu;
import javafx.scene.control.MenuBar;
import javafx.scene.control.MenuItem;
import javafx.scene.layout.Pane;
import javafx.scene.layout.FlowPane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.Scene;
import javafx.scene.shape.Rectangle;
import javafx.stage.Stage;
import javafx.scene.input.MouseEvent;

class BlockGrid
{
	public function BlockGrid()
	{
		GridPane blockGrid = new GridPane();
		for ( int i = 0; i < numberOfBlocks; i++ )
		{
			final int blockNumber = i;
			int y = ( int )( Math.floor( i / numberOfBlocksPerRow ) );
			int x = i % numberOfBlocksPerRow;
			Rectangle block = new Rectangle( ( double )( blockSize ), ( double )( blockSize ) );
			block.setFill( Color.TRANSPARENT );
			block.setStroke( Color.BLACK );
			block.setOnMouseClicked
			(
				e ->
				{
					System.out.println( blockNumber );
				}
			);
			blockGrid.add( block, y, x, 1, 1 );
		}
	}

	private final int numberOfBlocks = 336;
	private final int numberOfBlocksPerRow = 16;
	private final int blockSize = 16;
}
