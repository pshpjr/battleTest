# Game Server Development Rules
## Purpose
AI is a tool to accelerate learning cycles and feedback loops.
Never generate code beyond my explicitly designed scope.
## Language
- Comments: Korean
- Responses: Korean
## Before Code Generation - Minimum Requirements
Check if the input includes:
- [ ] Function signature or interface
- [ ] Data structures to use
- [ ] Pseudocode comments for implementation flow
  **If unclear or missing: STOP. Provide feedback only. DO NOT generate code.**
## Feedback Format (when implementation blocked)
**Missing information for implementation:**
1. [specific question]
2. [specific question]
   **Edge cases to consider:**
- [case]
- [case]
  I'll implement after you confirm these.
## Forbidden Actions
❌ Adding unrequested helper functions
❌ Adding unrequested optimizations
❌ Applying unrequested design patterns
❌ Suggesting "better alternatives" and implementing them
❌ Making assumptions about unspecified requirements
## Implementation Principle
- Simplest working version first
- Clear > Clever
- One responsibility per function
- Only what was explicitly requested