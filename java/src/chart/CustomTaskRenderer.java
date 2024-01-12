package chart;

import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;
import schedule.Task;

import com.flexganttfx.model.ActivityRef;
import com.flexganttfx.view.graphics.ActivityBounds;
import com.flexganttfx.view.graphics.GraphicsBase;
import com.flexganttfx.view.graphics.renderer.ActivityBarRenderer;
import com.flexganttfx.view.util.Position;

public class CustomTaskRenderer extends ActivityBarRenderer<Task> {

    public CustomTaskRenderer(GraphicsBase<?> graphics, String name) {
        super(graphics, name);
        setCornersRounded(false);
    }
    
    @Override
    protected ActivityBounds drawActivity(ActivityRef<Task> activityRef, Position position, GraphicsContext gc, double x, double y, double w, double h, boolean selected, boolean hover, boolean highlighted, boolean pressed) {
        Task task = activityRef.getActivity();

        Color taskColor;
        taskColor = task.getColor();
 
        gc.setFill(taskColor);
        gc.fillRect(x, y, w, h);

        // Draw the task ID on the range bar
        gc.setFill(Color.BLACK);
        //gc.fillText(task.getId(), x + 5, y + h / 2);  // To write the taskId number inside the range (not clear and visible)

        // Create and return the ActivityBounds object for this activity
        return new ActivityBounds(activityRef, x, y, w, h);
    }
    

} 
