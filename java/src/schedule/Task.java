package schedule;

import com.flexganttfx.model.activity.MutableActivityBase;

import javafx.scene.paint.Color;

import java.time.Instant;

public class Task extends MutableActivityBase<Integer> {
	
	private Color color;
	private int id;
	
    public Task(int id, Instant startTime, Instant endTime, Color taskcolor) {
    	setId(id);
        setUserObject(id);
        setName("Task " + id);
        setStartTime(startTime);
        setEndTime(endTime);
        setColor(taskcolor);
    }
    
    public Color getColor() {
        return color;
    }
    
    public void setId(int id) {
        this.id = id;
    }
    
    public int getId(Color color) {
        return this.id;
    }

    public void setColor(Color color) {
        this.color = color;
    }
}
